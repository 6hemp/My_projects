import unittest

from custom_list import CustomList


def is_equal(custom_list1, custom_list2, msg=None):
    if not list.__eq__(custom_list1, custom_list2):
        raise AssertionError(msg)


class TestHtmlParsing(unittest.TestCase):

    def setUp(self):
        self.custom_list_a = CustomList([1, 2, 3, 4])  # 10
        self.custom_list_b = CustomList([1, 4, 9, 16])  # 30
        self.custom_list_c = CustomList([-10, 14])  # 4
        self.custom_list_d = CustomList([11, -1])  # 10
        self.custom_list_e = CustomList([27, 1, 1, 1])  # 30
        self.list_b = [1, 4, 9, 16]
        self.list_c = [-10, 14]
        self.addTypeEqualityFunc(CustomList, is_equal)

    def tearDown(self):
        pass

    def test_operations_result_type(self):
        self.assertTrue(isinstance(self.custom_list_a + self.custom_list_b, CustomList))
        self.assertTrue(isinstance(self.custom_list_a + self.custom_list_c, CustomList))
        self.assertTrue(isinstance(self.custom_list_a + self.list_b, CustomList))
        self.assertTrue(isinstance(self.custom_list_a + self.list_c, CustomList))

        self.assertTrue(isinstance(self.custom_list_b + self.custom_list_a, CustomList))
        self.assertTrue(isinstance(self.custom_list_c + self.custom_list_a, CustomList))
        self.assertTrue(isinstance(self.list_b + self.custom_list_a, CustomList))
        self.assertTrue(isinstance(self.list_c + self.custom_list_a, CustomList))

        self.assertTrue(isinstance(self.custom_list_a - self.custom_list_b, CustomList))
        self.assertTrue(isinstance(self.custom_list_a - self.custom_list_c, CustomList))
        self.assertTrue(isinstance(self.custom_list_a - self.list_b, CustomList))
        self.assertTrue(isinstance(self.custom_list_a - self.list_c, CustomList))

        self.assertTrue(isinstance(self.custom_list_b - self.custom_list_a, CustomList))
        self.assertTrue(isinstance(self.custom_list_c - self.custom_list_a, CustomList))
        self.assertTrue(isinstance(self.list_b - self.custom_list_a, CustomList))
        self.assertTrue(isinstance(self.list_c - self.custom_list_a, CustomList))

    def test_add(self):
        self.assertEqual(self.custom_list_a + self.custom_list_b, CustomList([2, 6, 12, 20]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_b, CustomList([1, 4, 9, 16]))

        self.assertEqual(self.custom_list_a + self.custom_list_c, CustomList([-9, 16, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_c, CustomList([-10, 14]))

        self.assertEqual(self.custom_list_a + self.list_b, CustomList([2, 6, 12, 20]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.list_b, [1, 4, 9, 16])

        self.assertEqual(self.custom_list_a + self.list_c, CustomList([-9, 16, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_c, [-10, 14])

        self.assertEqual(self.custom_list_a + CustomList([]), CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))

        self.assertEqual(self.custom_list_a + [], CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))

    def test_add_invalid(self):
        self.assertRaises(TypeError, CustomList.__add__, self.custom_list_a, 1)
        self.assertRaises(TypeError, CustomList.__add__, self.custom_list_a, ['1', '2', '3', '4'])
        self.assertRaises(TypeError, CustomList.__add__, self.custom_list_a, None)
        self.assertRaises(TypeError, CustomList.__add__, self.custom_list_a, (1, 2, 3, 4))

    def test_radd(self):
        self.assertEqual(self.custom_list_b + self.custom_list_a, CustomList([2, 6, 12, 20]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_b, CustomList([1, 4, 9, 16]))

        self.assertEqual(self.custom_list_c + self.custom_list_a, CustomList([-9, 16, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_c, CustomList([-10, 14]))

        self.assertEqual(self.list_b + self.custom_list_a, CustomList([2, 6, 12, 20]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.list_b, [1, 4, 9, 16])

        self.assertEqual(self.list_c + self.custom_list_a, CustomList([-9, 16, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_c, [-10, 14])

        self.assertEqual(CustomList([]) + self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))

        self.assertEqual([] + self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))

    def test_radd_invalid(self):
        self.assertRaises(TypeError, CustomList.__radd__, 1, self.custom_list_a)
        self.assertRaises(TypeError, CustomList.__radd__, ['1', '2', '3', '4'], self.custom_list_a)
        self.assertRaises(TypeError, CustomList.__radd__, None, self.custom_list_a)
        self.assertRaises(TypeError, CustomList.__radd__, (1, 2, 3, 4), self.custom_list_a)

    def test_sub(self):
        self.assertEqual(self.custom_list_a - self.custom_list_b, CustomList([0, -2, -6, -12]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_b, CustomList([1, 4, 9, 16]))

        self.assertEqual(self.custom_list_a - self.custom_list_c, CustomList([11, -12, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_c, CustomList([-10, 14]))

        self.assertEqual(self.custom_list_a - self.list_b, CustomList([0, -2, -6, -12]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.list_b, [1, 4, 9, 16])

        self.assertEqual(self.custom_list_a - self.list_c, CustomList([11, -12, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_c, [-10, 14])

        self.assertEqual(self.custom_list_a - CustomList([]), CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))

        self.assertEqual(self.custom_list_a - [], CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))

    def test_sub_invalid(self):
        self.assertRaises(TypeError, CustomList.__sub__, self.custom_list_a, 1)
        self.assertRaises(TypeError, CustomList.__sub__, self.custom_list_a, ['1', '2', '3', '4'])
        self.assertRaises(TypeError, CustomList.__sub__, self.custom_list_a, None)
        self.assertRaises(TypeError, CustomList.__sub__, self.custom_list_a, (1, 2, 3, 4))

    def test_rsub(self):
        self.assertEqual(self.custom_list_b - self.custom_list_a, CustomList([0, 2, 6, 12]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_b, CustomList([1, 4, 9, 16]))

        self.assertEqual(self.custom_list_c - self.custom_list_a, CustomList([-11, 12, -3, -4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_c, CustomList([-10, 14]))

        self.assertEqual(self.list_b - self.custom_list_a, CustomList([0, 2, 6, 12]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.list_b, [1, 4, 9, 16])

        self.assertEqual(self.list_c - self.custom_list_a, CustomList([-11, 12, -3, -4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))
        self.assertEqual(self.custom_list_c, [-10, 14])

        self.assertEqual(CustomList([]) - self.custom_list_a, CustomList([-1, -2, -3, -4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))

        self.assertEqual([] - self.custom_list_a, CustomList([-1, -2, -3, -4]))
        self.assertEqual(self.custom_list_a, CustomList([1, 2, 3, 4]))

    def test_rsub_invalid(self):
        self.assertRaises(TypeError, CustomList.__rsub__, 1, self.custom_list_a)
        self.assertRaises(TypeError, CustomList.__rsub__, ['1', '2', '3', '4'], self.custom_list_a)
        self.assertRaises(TypeError, CustomList.__rsub__, None, self.custom_list_a)
        self.assertRaises(TypeError, CustomList.__rsub__, (1, 2, 3, 4), self.custom_list_a)

    def test_lt(self):
        self.assertTrue(self.custom_list_a < self.custom_list_b)
        self.assertTrue(self.custom_list_c < self.custom_list_a)
        self.assertFalse(self.custom_list_a < self.custom_list_d)
        self.assertFalse(self.custom_list_a < self.custom_list_c)

    def test_lt_invalid(self):
        self.assertRaises(TypeError, CustomList.__lt__, self.custom_list_a, 1)
        self.assertRaises(TypeError, CustomList.__lt__, self.custom_list_a, ['1', '2', '3', '4'])
        self.assertRaises(TypeError, CustomList.__lt__, self.custom_list_a, None)
        self.assertRaises(TypeError, CustomList.__lt__, self.custom_list_a, (1, 2, 3, 4))

    def test_le(self):
        self.assertTrue(self.custom_list_a <= self.custom_list_b)
        self.assertTrue(self.custom_list_a <= self.custom_list_d)
        self.assertFalse(self.custom_list_a <= self.custom_list_c)
        self.assertFalse(self.custom_list_b <= self.custom_list_c)

    def test_le_invalid(self):
        self.assertRaises(TypeError, CustomList.__le__, self.custom_list_a, 1)
        self.assertRaises(TypeError, CustomList.__le__, self.custom_list_a, ['1', '2', '3', '4'])
        self.assertRaises(TypeError, CustomList.__le__, self.custom_list_a, None)
        self.assertRaises(TypeError, CustomList.__le__, self.custom_list_a, (1, 2, 3, 4))

    def test_eq(self):
        self.assertTrue(self.custom_list_a == self.custom_list_d)
        self.assertTrue(self.custom_list_b == self.custom_list_e)
        self.assertFalse(self.custom_list_a == self.custom_list_b)
        self.assertFalse(self.custom_list_a == self.custom_list_c)

    def test_eq_invalid(self):
        self.assertRaises(TypeError, CustomList.__eq__, self.custom_list_a, 1)
        self.assertRaises(TypeError, CustomList.__eq__, self.custom_list_a, ['1', '2', '3', '4'])
        self.assertRaises(TypeError, CustomList.__eq__, self.custom_list_a, None)
        self.assertRaises(TypeError, CustomList.__eq__, self.custom_list_a, (1, 2, 3, 4))

    def test_ne(self):
        self.assertTrue(self.custom_list_a != self.custom_list_b)
        self.assertTrue(self.custom_list_a != self.custom_list_c)
        self.assertFalse(self.custom_list_a != self.custom_list_d)
        self.assertFalse(self.custom_list_b != self.custom_list_e)

    def test_ne_invalid(self):
        self.assertRaises(TypeError, CustomList.__ne__, self.custom_list_a, 1)
        self.assertRaises(TypeError, CustomList.__ne__, self.custom_list_a, ['1', '2', '3', '4'])
        self.assertRaises(TypeError, CustomList.__ne__, self.custom_list_a, None)
        self.assertRaises(TypeError, CustomList.__ne__, self.custom_list_a, (1, 2, 3, 4))

    def test_gt(self):
        self.assertTrue(self.custom_list_a > self.custom_list_c)
        self.assertTrue(self.custom_list_b > self.custom_list_c)
        self.assertFalse(self.custom_list_a > self.custom_list_b)
        self.assertFalse(self.custom_list_a > self.custom_list_d)

    def test_gt_invalid(self):
        self.assertRaises(TypeError, CustomList.__gt__, self.custom_list_a, 1)
        self.assertRaises(TypeError, CustomList.__gt__, self.custom_list_a, ['1', '2', '3', '4'])
        self.assertRaises(TypeError, CustomList.__gt__, self.custom_list_a, None)
        self.assertRaises(TypeError, CustomList.__gt__, self.custom_list_a, (1, 2, 3, 4))

    def test_ge(self):
        self.assertTrue(self.custom_list_a >= self.custom_list_d)
        self.assertTrue(self.custom_list_a >= self.custom_list_c)
        self.assertFalse(self.custom_list_a >= self.custom_list_b)
        self.assertFalse(self.custom_list_c >= self.custom_list_a)

    def test_ge_invalid(self):
        self.assertRaises(TypeError, CustomList.__ge__, self.custom_list_a, 1)
        self.assertRaises(TypeError, CustomList.__ge__, self.custom_list_a, ['1', '2', '3', '4'])
        self.assertRaises(TypeError, CustomList.__ge__, self.custom_list_a, None)
        self.assertRaises(TypeError, CustomList.__ge__, self.custom_list_a, (1, 2, 3, 4))

    def test_str(self):
        self.assertEqual(str(self.custom_list_a), '[1, 2, 3, 4]\tsum: 10')
        self.assertEqual(str(self.custom_list_b), '[1, 4, 9, 16]\tsum: 30')
        self.assertEqual(str(self.custom_list_c), '[-10, 14]\tsum: 4')
        self.assertEqual(str(self.custom_list_d), '[11, -1]\tsum: 10')
        self.assertEqual(str(self.custom_list_e), '[27, 1, 1, 1]\tsum: 30')


if __name__ == '__main__':
    unittest.main()
