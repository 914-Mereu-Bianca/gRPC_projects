#include <grpc++/grpc++.h>
#include <grpcpp/resource_quota.h>
#include "../include/server_impl.h"
#include <chrono>
#include <thread>
#include "server_impl.h"

ServiceImpl::ServiceImpl(const std::string &ip, uint16_t port)
    : ip_(ip), port_(port) {}


grpc::Status ServiceImpl::ProtoMethod(grpc::ServerContext *context, const data::Request* request, data::Response *response)
{
    response->set_response(request->request());
    
    std::this_thread::sleep_for(std::chrono::seconds(3));  // the request is taking longer, so we can see how many of them can run simultaneously

    std::cout<<request->request()<<std::endl;
    return grpc::Status();
}


void ServiceImpl::RunServer()
{

    std::string server_address = ip_ + ":" + std::to_string(port_);
    
    grpc::ServerBuilder builder;
    
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    
    // The code below sets the maximum number of threads that can run simultaneously 
    // This number will be updated only if it is less than the system-imposed limit on the number of threads that can be created

    grpc::ResourceQuota rq;
    rq.SetMaxThreads(20);
    builder.SetResourceQuota(rq);
    
    builder.RegisterService(this);

    std::unique_ptr<grpc::Server> server{builder.BuildAndStart()};

    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();

}

