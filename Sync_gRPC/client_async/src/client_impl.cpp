#include <grpc++/grpc++.h>
#include "../include/client_impl.h"

ClientImpl::ClientImpl(std::shared_ptr<grpc::Channel> channel)
: _stub(data::ProtoService::NewStub(channel))
{
    arr.resize(20000, 0);
}

void ClientImpl::ProtoMethod(const std::string &name)
{

    data::Request request;
    request.set_request(name);

    auto* call = new AsyncClientCall;

    call->rpc = _stub->PrepareAsyncProtoMethod(&call->context, request, &_queue);
        
    call->rpc->StartCall();
    
    call->rpc->Finish(&call->response, &call->status, (void*)call);
}

void ClientImpl::AsyncCompleteRPC()
{

    void* tag;
    bool ok = false;
    while (_queue.Next(&tag, &ok)) {
        if (!ok) {
            std::cerr << "Something went wrong" << std::endl;
            abort();
        }
        std::string err;
        auto* call = static_cast<AsyncClientCall*>(tag);
        if (call) {
            if (call->status.ok()) {
                arr[std::stoi(call->response.response())] = 1;
                //std::cout << "Client received: " << call->response.response() << std::endl;
            } else {
                //arr[std::stoi(call->response.response())] = 2;
                //std::cerr << call->status.error_code() << ": " << call->status.error_message() << std::endl;
                //std::cout << "Client received: RPC failed" << std::endl;
            }
        } else {
            err = "A client call was deleted";
        }
        delete call;
        if (!err.empty()) {
            throw std::runtime_error(err);            
        }
    }
}
