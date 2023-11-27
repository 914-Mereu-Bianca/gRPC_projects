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
    
    inline void setArrayValueAtPoz(int i, int val) { arr[i] = val; }

    inline int getArrayValue(int i) { return arr[i]; }

private:

    
    struct AsyncClientCall {
        data::Response response;
        grpc::ClientContext context;
        grpc::Status status;
        std::unique_ptr<grpc::ClientAsyncResponseReader<data::Response>> rpc;
    };
    
    std::unique_ptr<data::ProtoService::Stub> _stub;
    grpc::CompletionQueue _queue;

    // the following array will contain 1 on the positions of the requests that were successfully processed and 0 for the ones that were not
    std::vector<int> arr;

};

#endif  // _CLIENT_IMPL_H_