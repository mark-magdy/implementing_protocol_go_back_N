#include <bits/stdc++.h>
#include "PROTOCOL5.h"
using namespace std ;

vector<pair<Frame,Frame>> timeline(1e4); // (frames at sender ,  frames at reciver )

void to_physical_layer_to_receiver(Frame &frame) {
    //TODO: introduce error here 
    timeline[cur_time+PROPAGATION_DELAY].AT_RECEIVER=frame;
}
void to_physical_layer_to_sender(Frame &frame) {
    //TODO : introduce error here 
    timeline[cur_time+PROPAGATION_DELAY].AT_SENDER =frame;
}
void from_physical_layer_at_sender(Frame &frame) {
    frame = timeline[cur_time].AT_SENDER ;
}
void from_physical_layer_at_receiver(Frame &frame) {
    frame = timeline[cur_time].AT_RECEIVER ;
}

void network(){
    //
}
void sender(){
    //kirooo
    int x;
}
void receiver (){
    // 
}
int main() {
    cout << "PLEASE enter the number of frames you want to send\n";
    cin >> no_of_frames_to_send;
    for (cur_time = 0; cur_time < 1e6; ++cur_time) { // each step is a unit of time
        sender();
        receiver();
    }
    return 0;
}
