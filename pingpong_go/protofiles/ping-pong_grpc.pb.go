// Code generated by protoc-gen-go-grpc. DO NOT EDIT.
// versions:
// - protoc-gen-go-grpc v1.2.0
// - protoc             v3.21.9
// source: protofiles/ping-pong.proto

package grpc_microservices_

import (
	context "context"
	grpc "google.golang.org/grpc"
	codes "google.golang.org/grpc/codes"
	status "google.golang.org/grpc/status"
)

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
// Requires gRPC-Go v1.32.0 or later.
const _ = grpc.SupportPackageIsVersion7

// MathTestClient is the client API for MathTest service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://pkg.go.dev/google.golang.org/grpc/?tab=doc#ClientConn.NewStream.
type MathTestClient interface {
	// Function invoked to send the request
	SendRequest(ctx context.Context, in *MathRequest, opts ...grpc.CallOption) (*MathReply, error)
}

type mathTestClient struct {
	cc grpc.ClientConnInterface
}

func NewMathTestClient(cc grpc.ClientConnInterface) MathTestClient {
	return &mathTestClient{cc}
}

func (c *mathTestClient) SendRequest(ctx context.Context, in *MathRequest, opts ...grpc.CallOption) (*MathReply, error) {
	out := new(MathReply)
	err := c.cc.Invoke(ctx, "/mathtest.MathTest/sendRequest", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

// MathTestServer is the server API for MathTest service.
// All implementations must embed UnimplementedMathTestServer
// for forward compatibility
type MathTestServer interface {
	// Function invoked to send the request
	SendRequest(context.Context, *MathRequest) (*MathReply, error)
	mustEmbedUnimplementedMathTestServer()
}

// UnimplementedMathTestServer must be embedded to have forward compatible implementations.
type UnimplementedMathTestServer struct {
}

func (UnimplementedMathTestServer) SendRequest(context.Context, *MathRequest) (*MathReply, error) {
	return nil, status.Errorf(codes.Unimplemented, "method SendRequest not implemented")
}
func (UnimplementedMathTestServer) mustEmbedUnimplementedMathTestServer() {}

// UnsafeMathTestServer may be embedded to opt out of forward compatibility for this service.
// Use of this interface is not recommended, as added methods to MathTestServer will
// result in compilation errors.
type UnsafeMathTestServer interface {
	mustEmbedUnimplementedMathTestServer()
}

func RegisterMathTestServer(s grpc.ServiceRegistrar, srv MathTestServer) {
	s.RegisterService(&MathTest_ServiceDesc, srv)
}

func _MathTest_SendRequest_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(MathRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(MathTestServer).SendRequest(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/mathtest.MathTest/sendRequest",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(MathTestServer).SendRequest(ctx, req.(*MathRequest))
	}
	return interceptor(ctx, in, info, handler)
}

// MathTest_ServiceDesc is the grpc.ServiceDesc for MathTest service.
// It's only intended for direct use with grpc.RegisterService,
// and not to be introspected or modified (even as a copy)
var MathTest_ServiceDesc = grpc.ServiceDesc{
	ServiceName: "mathtest.MathTest",
	HandlerType: (*MathTestServer)(nil),
	Methods: []grpc.MethodDesc{
		{
			MethodName: "sendRequest",
			Handler:    _MathTest_SendRequest_Handler,
		},
	},
	Streams:  []grpc.StreamDesc{},
	Metadata: "protofiles/ping-pong.proto",
}
