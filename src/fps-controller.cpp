#include <chrono>
#include <thread>

std::chrono::_V2::system_clock::time_point start_tm;
std::chrono::_V2::system_clock::time_point end_tm;

int target_fps = 30;
double frame_time = 1.0 / target_fps;

void set_fps(int fps) {
    target_fps = fps;
    frame_time = 1.0 / fps;
}

void start_timer() {
    start_tm = std::chrono::high_resolution_clock::now();
}

void end_timer() {
    end_tm = std::chrono::high_resolution_clock::now();

    // aqui calcula quantos frames teve
    std::chrono::duration<double> elapsed = end_tm - start_tm;

    // espera o tempo restante para atingir o target FPS
    if (elapsed.count() < frame_time) {
        std::this_thread::sleep_for(std::chrono::duration<double>(frame_time - elapsed.count()));
    }
}