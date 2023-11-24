#include <grpc++/grpc++.h>
#include "../include/client_impl.h"

ClientImpl::ClientImpl(std::shared_ptr<grpc::Channel> channel)
: _stub(data::ProtoService::NewStub(channel))
{
    nr = 0;
}

void ClientImpl::ProtoMethod(const std::string &name)
{

    data::Request request;
    request.set_request(name);

    // send request
    data::Response response;
    grpc::ClientContext context;
    grpc::Status status;
    status = _stub->ProtoMethod(&context, request, &response);

    // Handle response
    if (status.ok()) {
        nr = 1;
        //std::cout<<response.response()<<std::endl;
    } else {
        nr = 2;
        //std::cerr << status.error_code() << ": " << status.error_message() << std::endl;
    }
    
}

