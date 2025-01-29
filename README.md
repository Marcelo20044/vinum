## Our alcohol-oriented programming language

#### Executes .vnm files
Array sorting example:
```
fun printArr(pinta arr[]) {
    pour pinta i = 0; i < len(arr); i = i + 1 {
        toast(arr[i] + " ")
    }
}

pinta arr[5] = {2, 5, 1, 3, 4} 
toast("before sort: ")
printArr(arr)

pour pinta i = 1; i < n; i = i + 1 {
    pour pinta j = i; j > 0; j = j - 1 {
        beef arr[j - 1] > arr[j] {
            pinta temp = arr[j - 1]
            arr[j - 1] = arr[j]
            arr[j] = temp
        }
    }
}

toast("\nafter sort: ")
printArr(arr)
```
have fun:)
