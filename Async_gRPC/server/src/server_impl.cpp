#include <grpc++/grpc++.h>
#include "../include/server_impl.h"
#include "src/cpp/server/dynamic_thread_pool.h"
#include <chrono>
#include <thread>
#include "server_impl.h"

ServiceImpl::ServiceImpl(const std::string &ip, uint16_t port)
    : ip_(ip), port_(port) {}


/*grpc::Status ServiceImpl::ProtoMethod(grpc::ServerContext *context, const data::Request* request, data::Response *response)
{
    response->set_message("Hello " + request->name());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return grpc::Status();
}*/


void ServiceImpl::RunServer()
{
    std::string server_address = ip_ + ":" + std::to_string(port_);
    
    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&_service);

    _queue = builder.AddCompletionQueue();
    _server = builder.BuildAndStart();

    std::cout << "Server listening on " << server_address << std::endl;
    HandleRPCs();

}

ServiceImpl::~ServiceImpl() {
    _server->Shutdown();
    _queue->Shutdown();
}


ServiceImpl::CallData::CallData(data::ProtoService::AsyncService *service, grpc::ServerCompletionQueue *queue)
    : _service{service}, _queue{queue}, _responder{&_context}, _status{CallStatus::CREATE} {
    Proceed();
}

void ServiceImpl::CallData::Proceed() {
    switch (_status) {
        case CallStatus::CREATE: {
            _service->RequestProtoMethod(&_context, &_request, &_responder, _queue, _queue, this);
            _status = CallStatus::PROCESS;
            break;
            }
        case CallStatus::PROCESS: {
            // Spawn a new CallData instance to serve new clients while we process
            // the one for this CallData. The instance will deallocate itself as
            // part of its FINISH state.
            new CallData{_service, _queue};

            //if(stoi(_request.request())%2) 
            //    for(int i = 0; i<=10000; i++) 
            //        for(int j = 0; j<=10000; j++)


            _response.set_response("Hello " + _request.request());
            std::cout<<_request.request()<<std::endl;
            
            _status = CallStatus::FINISH;
            _responder.Finish(_response, grpc::Status::OK, this);

            break;
        }
        default: {
            GPR_ASSERT(_status == CallStatus::FINISH);
            delete this;
        }
    }
}


void ServiceImpl::HandleRPCs() {
    new CallData{&_service, _queue.get()};
    void* tag;
    bool ok;
    while (true) {
        /*if (_queue->Next(&tag, &ok) && ok) {
            static_cast<CallData*>(tag)->Proceed();
        } else { 
            std::cerr << "Something went wrong" << std::endl;
            abort();
        }*/
        GPR_ASSERT(_queue->Next(&tag, &ok));
        GPR_ASSERT(ok);
        static_cast<CallData*>(tag)->Proceed();
    }
}