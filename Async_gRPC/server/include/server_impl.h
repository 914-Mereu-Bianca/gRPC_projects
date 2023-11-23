#ifndef _SERVER_IMPL_
#define _SERVER_IMPL_

#include "../../build/proto_generated/data.grpc.pb.h"

class ServiceImpl : public data::ProtoService::Service {
public:
    ServiceImpl(const std::string& ip, uint16_t port);
        
    void RunServer();

    ~ServiceImpl();

private:

    class CallData {
    public:
        CallData(data::ProtoService::AsyncService* service, grpc::ServerCompletionQueue* queue);

        void Proceed();
    
        data::ProtoService::AsyncService* _service;
        grpc::ServerCompletionQueue* _queue;
        grpc::ServerContext _context;
        data::Request _request;
        data::Response _response;
        grpc::ServerAsyncResponseWriter<data::Response> _responder;
        enum class CallStatus {
            CREATE, PROCESS, FINISH
        };
        CallStatus _status;
    };

    void HandleRPCs();

    //grpc::Status ProtoMethod(grpc::ServerContext* context, const data::Request* request, data::Response* response) override;
    
    data::ProtoService::AsyncService _service;
    std::unique_ptr<grpc::ServerCompletionQueue> _queue;
    std::unique_ptr<grpc::Server> _server;
    std::string ip_;
    uint16_t port_;
};

#endif // _SERVER_IMPL_