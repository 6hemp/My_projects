class LRUCache:
    def __init__(self, limit=42):
        if type(limit) is not int:
            raise TypeError('limit is not integer')
        if limit <= 0:
            raise ValueError('limit is not positive')

        self.__limit = limit
        self.__data = {}
        self.__counter = 0
        self.__uses = {}
        self.__uses_rev = {}

    def __update(self, key):
        if key in self.__uses_rev:
            self.__uses.pop(self.__uses_rev[key])
        self.__uses[self.__counter] = key
        self.__uses_rev[key] = self.__counter
        self.__counter += 1

    def __getitem__(self, key):
        if key not in self.__data:
            return None
        self.__update(key)
        return self.__data[key]

    def __setitem__(self, key, value):
        self.__data[key] = value
        self.__update(key)
        if len(self.__data) > self.__limit:
            lru_counter = min(self.__uses)
            self.__uses_rev.pop(self.__uses[lru_counter])
            self.__data.pop(self.__uses.pop(lru_counter))
