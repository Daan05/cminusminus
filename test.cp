fn some_func(start_val) {
    let res = start_val;

    let i = 0;
    while (i < 10) {
        let j = 0;
        while (j < 10) {
            res = res + 1;
            j = j + 1;
        }
        i = i + 1;
    }

    if (res == 100) {
        print res;
    } else {
        print -1;
    }
}

fn rec_func(n) {
    print n;

    if (n)
        rec_func(n - 1);
}

some_func(0); // prints 100
some_func(1); // prints -1

rec_func(10); // prints 10 9 8 7 6 5 4 3 2 1 0
