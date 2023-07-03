package main

import (
	"fmt"
	grpc_microservices "grpc_microservices/pingpong_client"
	"log"
)

func main() {
	result, err := grpc_microservices.DoRequest()
	if err != nil {
		log.Fatalf("fatal error: %v", err)
	}
	fmt.Printf("Calculated mult is: %d", result)
}
