package grpc_microservices

import (
	"context"
	"flag"
	"log"
	"time"

	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"

	pb "grpc_microservices/protofiles"
)

var (
	address          = flag.String("address", "localhost:50000", "server address")
	firstValueToSum  = flag.Int("a", 0, "first value to calculate sum")
	secondValueToSum = flag.Int("b", 0, "second value to calcualte sum")
)

func DoRequest() (int, error) {
	flag.Parse()

	conn, err := grpc.Dial(*address, grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		log.Fatal("can't connect to: %s, error: %v", *address, err)
	}

	c := pb.NewMathTestClient(conn)

	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	defer cancel()

	r, err := c.SendRequest(ctx, &pb.MathRequest{A: int32(*firstValueToSum), B: int32(*secondValueToSum)})
	if err != nil {
		log.Fatalf("can't do grpc request, error: %v", err)
	}

	return int(r.Result), nil
}
