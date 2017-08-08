#include "ofApp.h"

struct PeseudoRandom {
    virtual ~PeseudoRandom() {}
    
    virtual uint32_t generate() = 0;
    virtual double uniform() = 0;
    virtual double uniform(double a, double b) = 0;
};
struct Xor : public PeseudoRandom {
    Xor() {
        
    }
    Xor(uint32_t seed) {
        _y = std::max(seed, 1u);
    }
    
    // 0 <= x <= 0x7FFFFFFF
    uint32_t generate() {
        _y = _y ^ (_y << 13); _y = _y ^ (_y >> 17);
        uint32_t value = _y = _y ^ (_y << 5); // 1 ~ 0xFFFFFFFF(4294967295
        return value >> 1;
    }
    // 0.0 <= x < 1.0
    double uniform() {
        return double(generate()) / double(0x80000000);
    }
    double uniform(double a, double b) {
        return a + (b - a) * double(uniform());
    }
public:
    uint32_t _y = 2463534242;
};

static const int kSize = 512;
std::vector<int> fire;
Xor g_random;

//--------------------------------------------------------------
void ofApp::setup(){
    _image.allocate(kSize, kSize, OF_IMAGE_COLOR);
    fire.resize(kSize * kSize, 0);
    _colorMap.load("color.png");
    _colorMap.setImageType(OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::update() {
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    static float elapsed = 0.0f;
    elapsed += 1.0f / 30.0f;
    
    for(int x = 0 ; x < kSize ; ++x) {
        int y = kSize - 1;
        // fire[y * kSize + x] = g_random.generate() % 1000;
        fire[y * kSize + x] = ofNoise(x * 0.1, elapsed * 0.1) * 3000;
    }
    
    for(int y = 1 ; y < kSize - 1; ++y) {
        for(int x = 1 ; x < kSize - 1 ; ++x) {
            // int y_index = kSize - 1 - y;
            int y_index = y;
            
            int count = 3;
            int sum = fire[(y_index + 1) * kSize + x - 1] + fire[(y_index + 1) * kSize + x] + fire[(y_index + 1) * kSize + x + 1];
            if(y_index + 2 < kSize - 1) {
                count += 1;
                sum += fire[(y_index + 2) * kSize + x];
            }
            sum /= count;
            
            float noise = ofNoise(y_index * 0.05, x * 0.05, elapsed * 2.0f) * 0.5f + 0.3f;
            int value = sum - noise * 30.0f;
            fire[y_index * kSize + x] = std::max(value, 0);
        }
    }
    uint8_t *color = _colorMap.getPixels().getData();
    
    uint8_t *p = _image.getPixels().getData();
    for(int y = 1 ; y < kSize - 1; ++y) {
        for(int x = 1 ; x < kSize - 1 ; ++x) {
            for(int i = 0; i < 3; ++i) {
                int index = y * kSize + x;
                int v = fire[index] ;
                int color_index = 256 - 2 -  std::min(fire[index] / 8, 255);
                color_index = std::max(color_index, 0);
                p[index * 3 + i] = color[color_index * 3 + i];
            }
        }
    }
    _image.update();
    _image.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
