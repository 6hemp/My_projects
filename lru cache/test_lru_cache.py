import unittest

from lru_cache import LRUCache


def do_nothing(*args, **kwargs):
    pass


class TestHtmlParsing(unittest.TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_limit(self):
        for lim in (10, 100, 2, 1):
            cache = LRUCache(lim)
            for i in range(lim):
                cache[i] = i * i
                for j in range(i + 1):
                    self.assertEqual(cache[j], j * j)
            cache[lim] = lim * lim
            self.assertEqual(cache[lim], lim * lim)
            inside = [cache[i] is not None for i in range(lim)]
            self.assertFalse(inside[0])
            self.assertTrue(all(inside[1:]))

    def test_complete_displacement(self):
        lim = 6
        cache = LRUCache(lim)
        for i in range(lim):
            cache[i] = i * i
        for i in range(lim):
            cache[str(i)] = i - 1
        for i in range(lim):
            self.assertIsNone(cache[i])
            self.assertEqual(cache[str(i)], i - 1)

    def test_default(self):
        cache = LRUCache(2)

        cache["k1"] = "val1"
        cache["k2"] = "val2"

        self.assertIsNone(cache["k3"])
        self.assertEqual(cache["k2"], "val2")
        self.assertEqual(cache["k1"], "val1")

        cache["k3"] = "val3"

        self.assertEqual(cache["k3"], "val3")
        self.assertIsNone(cache["k2"])
        self.assertEqual(cache["k1"], "val1")

    def test_delete(self):
        cache = LRUCache(10)
        for i in range(10):
            cache[i] = i * i
        cache[10] = 10 * 10
        self.assertIsNone(cache[0])
        cache[23] = 23 * 23
        self.assertIsNone(cache[1])
        for i in range(2, 10):
            self.assertEqual(cache[i], i * i)

        cache = LRUCache(10)
        for i in range(10):
            cache[i] = i * i
        for i in range(9, -1, -1):
            cache[i] = i * i
        cache[10] = 10 * 10
        self.assertIsNone(cache[9])
        cache[23] = 23 * 23
        self.assertIsNone(cache[8])
        for i in range(0, 8):
            self.assertEqual(cache[i], i * i)

        cache = LRUCache(10)
        for i in range(10):
            cache[i] = i * i
        for i in range(9, -1, -1):
            do_nothing(cache[i])
        cache[10] = 10 * 10
        self.assertIsNone(cache[9])
        cache[23] = 23 * 23
        self.assertIsNone(cache[8])
        for i in range(0, 8):
            self.assertEqual(cache[i], i * i)

        cache = LRUCache(10)
        for i in range(10):
            cache[i] = i * i
        cache[2] = -2
        for i in range(9):
            cache[str(i)] = i - 1
        self.assertEqual(cache[2], -2)
        for i in range(10):
            cache[str(i)] = i - 1
        self.assertIsNone(cache[2])

    def test_errors(self):
        self.assertRaises(TypeError, LRUCache, True)
        self.assertRaises(TypeError, LRUCache, 5.0)
        self.assertRaises(TypeError, LRUCache, (12,))
        self.assertRaises(TypeError, LRUCache, {'a': 7})

        self.assertRaises(ValueError, LRUCache, 0)
        self.assertRaises(ValueError, LRUCache, -1)
        self.assertRaises(ValueError, LRUCache, -10)

        cache = LRUCache(10)
        with self.assertRaises(TypeError):
            cache[[10]] = 5
        with self.assertRaises(TypeError):
            cache[{10}] = 5
        with self.assertRaises(TypeError):
            cache[{10: 10}] = 5

        cache[(10, 5)] = 15
        cache['10'] = 5
        cache[frozenset(range(10))] = [0, 1, 2, 3, 4]


if __name__ == '__main__':
    unittest.main()
