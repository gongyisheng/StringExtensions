import bm

def test_match():
    assert(bm.match("ddd", "ddd")==0)
    assert(bm.match("dddd", "ddd")==0)
    assert(bm.match("abcde", "abc")==0)
    assert(bm.match("ecdab", "ab")==3)
    assert(bm.match("ecdabababababc", "abc")==11)
    assert(bm.match("asjdiooeac", "X")==-1)
    assert(bm.match("awsxedc", "awsxedcs")==-1)
    print("test passed")

if __name__ == "__main__":
    test_match()