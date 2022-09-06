import unittest
from unittest.mock import patch
from faker import Faker

from html_parsing import parse_html


class TestHtmlParsing(unittest.TestCase):

    def setUp(self):
        self.fake = Faker()
        self.open_tag_dict = {}
        self.data_dict = {}
        self.close_tag_dict = {}

    def tearDown(self):
        pass

    def open_tag_callback(self, tag):
        if tag in self.open_tag_dict:
            self.open_tag_dict[tag] += 1
        else:
            self.open_tag_dict[tag] = 1

    def data_callback(self, data):
        if data in self.data_dict:
            self.data_dict[data] += 1
        else:
            self.data_dict[data] = 1

    def close_tag_callback(self, tag):
        if tag in self.close_tag_dict:
            self.close_tag_dict[tag] += 1
        else:
            self.close_tag_dict[tag] = 1

    @patch('test_html_parsing.TestHtmlParsing.close_tag_callback')
    def test_parse_html_mock(self, close_tag_callback_mock):
        """
        для проверки на покрытие с помошью coverage использовать
        coverage run -m unittest test_html_parsing
        иначе mock.patch игнорируется
        """
        close_tag_count = 0

        html_str = f'<a><b><c>{self.fake.text()}</c><c><d>{self.fake.text()}</d>{self.fake.text()}</c></b></a>'
        parse_html(html_str, self.open_tag_callback, self.data_callback, self.close_tag_callback)
        close_tag_count += 5
        self.assertEqual(close_tag_callback_mock.call_count, close_tag_count)
        self.assertEqual(len(self.close_tag_dict), 0)

        html_str = f'<a><b>{self.fake.text()}</b><b>{self.fake.text()}</b><b></b><b>{self.fake.text()}</b></a>'
        parse_html(html_str, self.open_tag_callback, self.data_callback, self.close_tag_callback)
        close_tag_count += 5
        self.assertEqual(close_tag_callback_mock.call_count, close_tag_count)
        self.assertEqual(len(self.close_tag_dict), 0)

        html_str = f'<a><b><c><b></b><d></d></c><c></c></b></a>'
        parse_html(html_str, self.open_tag_callback, self.data_callback, self.close_tag_callback)
        close_tag_count += 6
        self.assertEqual(close_tag_callback_mock.call_count, close_tag_count)
        self.assertEqual(len(self.close_tag_dict), 0)

        html_str = f'<a>{self.fake.text()}</a>'
        parse_html(html_str, self.open_tag_callback, self.data_callback, self.close_tag_callback)
        close_tag_count += 1
        self.assertEqual(close_tag_callback_mock.call_count, close_tag_count)
        self.assertEqual(len(self.close_tag_dict), 0)

    def test_parse_html_open_call(self):
        html_str = f'<a><b><c>{self.fake.text()}</c><c><d>{self.fake.text()}</d>{self.fake.text()}</c></b></a>'
        parse_html(html_str, self.open_tag_callback, None, None)
        self.assertEqual(sum(self.open_tag_dict.values()), 5)

        self.open_tag_dict = {}
        html_str = f'<a><b>{self.fake.text()}</b><b>{self.fake.text()}</b><b></b><b>{self.fake.text()}</b></a>'
        parse_html(html_str, self.open_tag_callback, None, None)
        self.assertEqual(sum(self.open_tag_dict.values()), 5)

        self.open_tag_dict = {}
        html_str = f'<a><b><c><b></b><b></b></c><c></c></b></a>'
        parse_html(html_str, self.open_tag_callback, None, None)
        self.assertEqual(self.open_tag_dict['b'], 3)

        self.open_tag_dict = {}
        html_str = f'<a>{self.fake.text()}</a>'
        parse_html(html_str, self.open_tag_callback, None, None)
        self.assertEqual(self.open_tag_dict['a'], 1)

    def test_parse_html_data_call(self):
        html_str = f'<a><b><c>{self.fake.text()}</c><c><d>{self.fake.text()}</d>{self.fake.text()}</c></b></a>'
        parse_html(html_str, None, self.data_callback, None)
        self.assertEqual(sum(self.data_dict.values()), 3)

        self.data_dict = {}
        html_str = f'<a><b>{self.fake.text()}</b><b>{self.fake.text()}</b><b></b><b>{self.fake.text()}</b></a>'
        parse_html(html_str, None, self.data_callback, None)
        self.assertEqual(sum(self.data_dict.values()), 3)

        self.data_dict = {}
        html_str = f'<a><b><c><b></b><b></b></c><c></c></b></a>'
        parse_html(html_str, None, self.data_callback, None)
        self.assertEqual(sum(self.data_dict.values()), 0)

        self.data_dict = {}
        html_str = f'<a>{self.fake.text()}</a>'
        parse_html(html_str, None, self.data_callback, None)
        self.assertEqual(sum(self.data_dict.values()), 1)

    def test_parse_html_close_call(self):
        html_str = f'<a><b><c>{self.fake.text()}</c><c><d>{self.fake.text()}</d>{self.fake.text()}</c></b></a>'
        parse_html(html_str, None, None, self.close_tag_callback)
        self.assertEqual(sum(self.close_tag_dict.values()), 5)

        self.close_tag_dict = {}
        html_str = f'<a><b>{self.fake.text()}</b><b>{self.fake.text()}</b><b></b><b>{self.fake.text()}</b></a>'
        parse_html(html_str, None, None, self.close_tag_callback)
        self.assertEqual(sum(self.close_tag_dict.values()), 5)

        self.close_tag_dict = {}
        html_str = f'<a><b><c><b></b><b></b></c><c></c></b></a>'
        parse_html(html_str, None, None, self.close_tag_callback)
        self.assertEqual(self.close_tag_dict['b'], 3)

        self.close_tag_dict = {}
        html_str = f'<a>{self.fake.text()}</a>'
        parse_html(html_str, None, None, self.close_tag_callback)
        self.assertEqual(self.close_tag_dict['a'], 1)

    def test_parse_html_type_error(self):
        self.assertRaises(TypeError, parse_html, 0)
        self.assertRaises(TypeError, parse_html, ['<html>', '</html>'])
        self.assertRaises(TypeError, parse_html, '<html></html>', 42, None, None)
        self.assertRaises(TypeError, parse_html, '<html></html>', None, 69, None)
        self.assertRaises(TypeError, parse_html, '<html></html>', None, None, 146)

    def test_parse_html_value_error(self):
        self.assertRaises(ValueError, parse_html, '<html>')
        self.assertRaises(ValueError, parse_html, f'<html><body>{self.fake.text()}</body>')
        self.assertRaises(ValueError, parse_html, f'<html><body>{self.fake.text()}</html>')
        self.assertRaises(ValueError, parse_html, f'<html><head><body>{self.fake.text()}</head></body></html>')


if __name__ == "__main__":
    unittest.main()
