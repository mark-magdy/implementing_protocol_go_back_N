/**
CODE WRITTEN BY (alphabetically):
 Jessica Fady,
 Kirolos Morcos,
 Mark Magdy,
 Youstina Magdy

 **/

#include <bits/stdc++.h>
#include "PROTOCOL5.h"

using namespace std;

/**  Description :
 local variable "frame" keeps getting updated when passed between layers
 THE LINES CARRYING DATA IS REPRESENTED BY QUEUES
**/

vector<pair<Frame, Frame>> timeline(1e4); // (frames at sender ,  frames at reciver ) //models the physical wire
// aux functions
int generateRandomInt(int lower, int upper) {
    static random_device rd;                        // Seed only once
    static mt19937 gen(rd());                       // Mersenne Twister engine
    uniform_int_distribution<> dist(lower, upper);  // Distribution defined for range
    return dist(gen);                               // Generate random number
}

void print_queue(queue<string> newQueue) {
    queue<string> tempQueue = newQueue; // Copy of the queue for printing
    while (!tempQueue.empty()) {
        cout << tempQueue.front() << " ";
        tempQueue.pop();
    }
    cout << "\"" << endl;
}

void increase(seq_nr &x) {
    x++;
    x = x % (MAX_SEQ + 1);
}

bool between(seq_nr a, seq_nr b, seq_nr c) {
    //return true if a<=b and b<c else false
    if ((a <= b) && (b < c))return 1;
    else return 0;
}

// physical layer functions
void to_physical_layer_to_receiver(Frame frame) {           // send frame form sender -> to receiver
    int random_error = generateRandomInt(1, 100);
    if (random_error % 9 == 0) {
        frame.check = zero;   //error bit
    }
    if (random_error % 6 == 0) {
        frame.kind = notframe; //corrupted frame
    }
    timeline[cur_time + PROPAGATION_DELAY].AT_RECEIVER = frame;
}

void to_physical_layer_to_sender(Frame frame) {            // send frame form receiver -> to sender
    int random_error = generateRandomInt(1, 100);
    if (random_error % 9 == 0) {
        frame.check = zero;   //error bit
    }
    if (random_error % 6 == 0) {
        frame.kind = notframe;   //corrupted frame
    }
    timeline[cur_time + PROPAGATION_DELAY].AT_SENDER = frame;
}

void from_physical_layer_at_sender(Frame &frame) {          // receive frame from physical layer at sender
    frame = timeline[cur_time].AT_SENDER;
}

void from_physical_layer_at_receiver(Frame &frame) {       // receive frame from physical layer at receiver
    frame = timeline[cur_time].AT_RECEIVER;
}

bool physical_to_sender_ready() {
    Frame f;
    from_physical_layer_at_sender(f);
    if (f.kind == ack)return 1;
    else return 0;
}

// network layer functions
packet from_network_layer_at_sender() {                 // send packet from network layer -> datalink Layer at sender
    if (!all_packets_network.empty()) {
        packet firstElement = all_packets_network.front();
        all_packets_network.pop();
        return firstElement;
    } else {
        return "";
    }
}

void to_network_layer_at_receiver(const packet &str) {  //send packet from datalink layer -> network Layer at receiver
    all_packets_reciver.push(str); // Push the string
}

bool network_is_ready() {
    return !all_packets_network.empty();
}

// timer functions
void start_timer(seq_nr s) {
    timers[s] = cur_time;
}

void stop_timer(seq_nr r) {
    timers[r] = -1;
}

bool timeout() {
    if (timers[ack_epected] == -1)return 0;
    if ((cur_time - timers[ack_epected]) >= TIMER_TIMOUT) {
        // cout<<cur_time<<" "<<timers[ack_epected]<<endl;
        return 1;
    }
    return 0;
}

// sender & receiver
void send(seq_nr f_n, packet f_info) {
    Frame s;
    s.seq = f_n;
    s.kind = info;
    s.info = f_info;
    s.check = one;
    to_physical_layer_to_receiver(s);
    start_timer(s.seq);
}

void sender() {
    if (retransmissions) {
        send(next_send, buffer[next_send]);
        cout << "At Sender : sending frame no > " << next_send << " < with msg '" << buffer[next_send] << "'" << endl;
        increase(next_send);
        retransmissions--;
    } else if (nbuffer && timeout()) {
        cout << "At sender: timeout " << timers[ack_epected] << endl;
        next_send = ack_epected;
        send(next_send, buffer[next_send]);
        cout << "At Sender : sending frame no > " << next_send << " < with msg '" << buffer[next_send] << "'" << endl;
        increase(next_send);
        retransmissions = nbuffer - 1;
        cout << ack_epected << " " << next_send << endl;
    } else if (network_is_ready()) {
        buffer[next_send] = from_network_layer_at_sender();
        nbuffer++;
        send(next_send, buffer[next_send]);
        cout << "At Sender : sending frame no > " << next_send << " < with msg '" << buffer[next_send] << "'" << endl;
        increase(next_send);
    }

    if (physical_to_sender_ready()) {
        Frame r;
        from_physical_layer_at_sender(r);
        cout << "At Sender: receiving ack frame " << r.ack << " " << "expected " << ack_epected << endl;
        while (between(ack_epected, r.ack, next_send)) {
            nbuffer--;
            stop_timer(ack_epected);
            increase(ack_epected);
        }
        //  cout<<ack_epected<<endl;
    }

}

void receiver() {
    static int Error_Discard_upcoming_frames = 0;
    Frame received_frame;
    Frame feedback;
    from_physical_layer_at_receiver(received_frame); //received_frame now is updated by physical layer
    if (received_frame.kind == info) {
        if (received_frame.seq == expected_to_be_received && received_frame.check == 1) { //wire pair (sent, ack)
            //send ack
            cout << "At Receiver: sending to sender Ack no " << received_frame.seq << endl;
            Error_Discard_upcoming_frames = 0;
            expected_to_be_received++;
            feedback.kind = ack;
            feedback.ack = received_frame.seq;
            to_network_layer_at_receiver(received_frame.info);
            to_physical_layer_to_sender(feedback);
        } else if (Error_Discard_upcoming_frames == 1) { // wire pair (sent,nack)
            cout << "At Receiver : Discarded!!" << received_frame.seq << endl;
            feedback.kind = nak;
            feedback.ack = received_frame.seq;
            to_physical_layer_to_sender(feedback);
        } else if (received_frame.seq != expected_to_be_received ||
                   received_frame.check == 0) { // wire pair (sent,discard)
            cout << "At Receiver: Error !! " << endl;
            Error_Discard_upcoming_frames = 1;
        }

    } else if (received_frame.kind != info) {
//        cout<<"At Receiver: No Frames Received !! "<<endl; //for testing
    }
}

// network layer
bool network(const vector<string> &data_from_input, int &index, unordered_set<int> &done) {
    if (done.size() == data_from_input.size()) {
        return false; //  enqueueing is complete
    }
    if (cur_time % frame_time == 0) {
        // Check if the current index has already been processed
        if (done.find(index) == done.end()) {
            // Enqueue the string
            all_packets_network.push(data_from_input[index]);
            done.insert(index);
        }
        index = (index + 1) % data_from_input.size();
    }
    return true;
}

void networkInterface(int &index, unordered_set<int> &done) {
    if (!stopNetwork && !network(data_from_input, index, done)) {
        // Exit the loop if all strings are processed
        stopNetwork = true;
        return;
    }
}

int main() {
    cout << "PLEASE enter the number of frames you want to send\n";
    cin >> no_of_frames_to_send;

    cout << "PLEASE enter the propagation delay in physical layer\n";
    cin >> PROPAGATION_DELAY;
    TIMER_TIMOUT = 4 * PROPAGATION_DELAY + 1;

    cout << "PLEASE enter the time of frames you want to send\n";
    cin >> frame_time;
    cin.ignore();
    cout << "PLEASE enter the frames\n";
    for (int i = 0; i < no_of_frames_to_send; i++) {
        getline(cin, input);
        data_from_input.push_back(input);
    }

    int index = 0;
    unordered_set<int> done;
    // each step is a unit of time
    for (cur_time = 0; cur_time < 1e4 && all_packets_reciver.size() < no_of_frames_to_send; ++cur_time) {
        networkInterface(index, done);

        cout << ">>  curent time is " << cur_time << endl;
        cout << "- Network-Sender's Packets: \"";
        print_queue(all_packets_network);

        sender();
        receiver();

        cout << "- Network-Receiver's Packets: \"";
        print_queue(all_packets_reciver);
        cout << endl;
    }
    return 0;
}
