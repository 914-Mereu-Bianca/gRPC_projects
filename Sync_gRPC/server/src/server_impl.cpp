#include <grpc++/grpc++.h>
#include "../include/server_impl.h"
#include "src/cpp/server/dynamic_thread_pool.h"
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <chrono>
#include <thread>
#include "server_impl.h"

ServiceImpl::ServiceImpl(const std::string &ip, uint16_t port)
    : ip_(ip), port_(port) {}


grpc::Status ServiceImpl::ProtoMethod(grpc::ServerContext *context, const data::Request* request, data::Response *response)
{
    response->set_response("Hello " + request->request());
    std::cout<<request->request()<<std::endl;
    //if(stoi(request->request())%2)
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return grpc::Status();
}


void ServiceImpl::RunServer()
{

    std::string server_address = ip_ + ":" + std::to_string(port_);
    
    grpc::ServerBuilder builder;
    

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    
    //grpc::ResourceQuota rq;
    //rq.Resize(4);
    //builder.SetResourceQuota(rq);
    
    builder.RegisterService(this);

    std::unique_ptr<grpc::Server> server{builder.BuildAndStart()};

    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();

}

