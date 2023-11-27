#include "include/client_impl.h"
#include <grpc++/grpc++.h>
#include <vector>
#include <string>
#include <limits>

int main() {

    ClientImpl client{grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())};

    std::thread thread{&ClientImpl::AsyncCompleteRPC, &client};
    
    int n = 40;

    for (int i = 0; i < n; ++i) {   
        std::string request{std::to_string(i)};
        
        client.ProtoMethod(request);
    }

    std::this_thread::sleep_for(std::chrono::seconds(10)); // wait 10 seconds before checking the successfully requests because the code is blocked forever

    int count = 0;

    for(int i = 0; i < n; i++){
        std::cout<<client.getArrayValue(i)<<" ";
        if (client.getArrayValue(i))
            count++;
    }

    std::cout << std::endl << "The nr of clients that send the request successfully is: " << count << std::endl;
    
    thread.join();

    
    return 0;
}