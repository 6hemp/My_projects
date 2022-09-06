import inspect


def is_list(func):
    def wrapper(*args, **kwargs):
        if not isinstance(args[0], list) or not isinstance(args[1], list):
            args_names = inspect.getfullargspec(func)[0]
            raise TypeError(f'{args_names[0]} and {args_names[1]} must be lists of int')
        return func(*args, **kwargs)
    return wrapper


class CustomList(list):

    @is_list
    def __add__(self, other):
        len_s, len_o = len(self), len(other)
        min_len, max_len = min(len_s, len_o), max(len_s, len_o)
        ans = CustomList([0] * max_len)
        for i in range(min_len):
            ans[i] += self[i] + other[i]
        if len_s < len_o:
            for i in range(min_len, max_len):
                ans[i] += other[i]
        else:
            for i in range(min_len, max_len):
                ans[i] += self[i]
        return ans

    @is_list
    def __sub__(self, other):
        len_s, len_o = len(self), len(other)
        min_len, max_len = min(len_s, len_o), max(len_s, len_o)
        ans = CustomList([0] * max_len)
        for i in range(min_len):
            ans[i] += self[i] - other[i]
        if len_s < len_o:
            for i in range(min_len, max_len):
                ans[i] -= other[i]
        else:
            for i in range(min_len, max_len):
                ans[i] += self[i]
        return ans

    @is_list
    def __radd__(self, other):
        return self + CustomList(other)

    @is_list
    def __rsub__(self, other):
        return CustomList(other) - self

    @is_list
    def __lt__(self, other):
        return sum(self) < sum(other)

    @is_list
    def __le__(self, other):
        return sum(self) <= sum(other)

    @is_list
    def __eq__(self, other):
        return sum(self) == sum(other)

    @is_list
    def __ne__(self, other):
        return sum(self) != sum(other)

    @is_list
    def __gt__(self, other):
        return sum(self) > sum(other)

    @is_list
    def __ge__(self, other):
        return sum(self) >= sum(other)

    def __str__(self):
        return f'{super().__str__()}\tsum: {sum(self)}'
