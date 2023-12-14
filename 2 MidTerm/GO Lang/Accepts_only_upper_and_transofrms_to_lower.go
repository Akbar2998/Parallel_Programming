package main

import (
    "bufio"
    "fmt"
    "os"
    "sync"
    "unicode"
)

func main() {
    var wg sync.WaitGroup
    ch := make(chan int)

    // Goroutine 1: Filters [A-Z] characters
    wg.Add(1)
    go func() {
        defer wg.Done()
        reader := bufio.NewReader(os.Stdin)
        for {
            char, _, err := reader.ReadRune()
            if err != nil {
                close(ch)
                return
            }
            if char == 'q' {
                close(ch)
                return
            }
            if unicode.IsUpper(char) {
                ch <- int(char)
            }
        }
    }()

    // Goroutine 2: Converts uppercase to lowercase
    wg.Add(1)
    go func() {
        defer wg.Done()
        for char := range ch {
            fmt.Printf("%c", unicode.ToLower(rune(char)))
        }
    }()

    wg.Wait()
}
