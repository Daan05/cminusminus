# C+ : A programming lnaguage

The only datatype is a 64 bit integer

## Example

``` rust
let res = 0;

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
```
