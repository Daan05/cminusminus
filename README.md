# C+ : A programming lnaguage

The only datatype is a 64 bit integer

## Example

``` rust
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

some_func(0); // prints 100
some_func(1); // prints -1
```
