#include "include/client_impl.h"
#include <grpc++/grpc++.h>
#include <vector>
#include <string>

int main() {

    ClientImpl client{grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())};

    std::thread thread{&ClientImpl::AsyncCompleteRPC, &client};
    
    for (int i = 0; i < 3450; ++i) {
        std::string request{std::to_string(i)};
        
        client.ProtoMethod(request);
    }

    thread.join();

    return 0;
}