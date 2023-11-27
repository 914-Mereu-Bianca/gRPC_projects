#ifndef _SERVER_IMPL_
#define _SERVER_IMPL_

#include "../../build/proto_generated/data.grpc.pb.h"

class ServiceImpl : public data::ProtoService::Service {
public:
    ServiceImpl(const std::string& ip, uint16_t port);
        
    void RunServer();

private:

    // This method is called when the client makes the request
    grpc::Status ProtoMethod(grpc::ServerContext* context, const data::Request* request, data::Response* response) override;
    
    std::string ip_;
    uint16_t port_;
};

#endif // _SERVER_IMPL_