#include "include/client_impl.h"
#include <grpc++/grpc++.h>
#include <vector>
#include <string>
#include <limits>

int main() {

    size_t max_size = std::numeric_limits<size_t>::max();

    std::cout << "Maximum value for size_t: " << max_size << std::endl;

    ClientImpl client{grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())};

    std::thread thread{&ClientImpl::AsyncCompleteRPC, &client};
    
    for (int i = 0; i < 3000; ++i) {   //3425
        std::string request{std::to_string(i)};
        
        client.ProtoMethod(request);
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    int i;

    for(i = 0; i < 3000; i++){
        std::cout<<client.arr[i]<<" ";
    }
    std::cout<<std::endl;

    for(i = 19999; i > 0; i--){
        if (client.arr[i] == true)
            break;
    }
    std::cout<<i<<std::endl;
    
    thread.join();

    
    return 0;
}