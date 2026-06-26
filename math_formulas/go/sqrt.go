package main

import (
	"fmt"
)

func Sqrt(x float64) float64 {
	z := 1.0
	temp := 0.0
	i := 0
	for {
		sum := temp - z
		if sum < 0.0 {
			sum = sum * -1
		}
		if sum < 0.000000001 {
			break
		}
		temp = z
		z -= (z*z - x) / (2 * z)
		fmt.Printf(" iteration [%d] : %f\n", i, z)
		i++
	}
	if z < 0.0 {
		return -z
	} else {
		return z
	}
}

func main() {
	fmt.Println(Sqrt(4))
}
