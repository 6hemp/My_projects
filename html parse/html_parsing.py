def parse_html(html_str, open_tag_callback=None, data_callback=None, close_tag_callback=None):
    if not isinstance(html_str, str):
        raise TypeError('html_str must be str')
    if open_tag_callback is not None and not callable(open_tag_callback):
        raise TypeError('open_tag_callback must be callable')
    if data_callback is not None and not callable(data_callback):
        raise TypeError('data_tag_callback must be callable')
    if close_tag_callback is not None and not callable(close_tag_callback):
        raise TypeError('close_tag_callback must be callable')
    stack = []
    html_str = ''.join(html_str.split('\n'))
    html_str = ''.join(html_str.split('\r'))
    html_str = ''.join(html_str.split('\t'))
    stage = 0
    cur_str = []
    for char in html_str:
        if stage == 0:
            if char != '<':
                cur_str.append(char)
            else:
                if len(cur_str) > 0:
                    if data_callback is not None:
                        data_callback(''.join(cur_str))
                    cur_str = []
                stage = 1
        elif stage == 1:
            if char != '/':
                cur_str.append(char)
                stage = 2
            else:
                stage = 3
        elif stage == 2:
            if char != '>':
                cur_str.append(char)
            else:
                stack.append(''.join(cur_str))
                if len(cur_str) > 0:
                    if open_tag_callback is not None:
                        open_tag_callback(''.join(cur_str))
                    cur_str = []
                stage = 0
        elif stage == 3:
            if char != '>':
                cur_str.append(char)
            else:
                if stack[-1] != ''.join(cur_str):
                    raise ValueError('invalid html_str')
                stack.pop()
                if len(cur_str) > 0:
                    if close_tag_callback is not None:
                        close_tag_callback(''.join(cur_str))
                    cur_str = []
                stage = 0
    if len(stack) != 0:
        raise ValueError('invalid html_str')
