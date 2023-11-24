#ifndef _CLIENT_IMPL_H_
#define _CLIENT_IMPL_H_

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include "../../build/proto_generated/data.grpc.pb.h"

class ClientImpl {

public:

    ClientImpl(std::shared_ptr<grpc::Channel> channel);
            
    void ProtoMethod(const std::string &name);

    void AsyncCompleteRPC();
    
    std::vector<int> arr;
private:

    
    struct AsyncClientCall {
        data::Response response;
        grpc::ClientContext context;
        grpc::Status status;
        std::unique_ptr<grpc::ClientAsyncResponseReader<data::Response>> rpc;
    };
    
    std::unique_ptr<data::ProtoService::Stub> _stub;
    grpc::CompletionQueue _queue;
    

};

#endif  // _CLIENT_IMPL_H_