// #pragma once

#ifndef GVE_SPRITE_H
#define GVE_SPRITE_H

#include <structs/spatial.h>

namespace assets{
    sf::Sprite &getSp(const string name);
    string getSpriteName(sf::Sprite &sprite);
};

#define getSprite assets::getSp

struct Animation{
    sf::Sprite *sheet;

    std::vector<sf::IntRect> frame_rects;
    std::vector<v2f> frame_origins;

    float time;
    float fps;

    bool playing = false;

    Animation():
        fps(0.f),
        sheet(nullptr)
    {}

    Animation(
        float fps, 
        sf::Sprite* sheet, 
        std::vector<sf::IntRect> frame_rects, 
        std::vector<v2f> frame_origins
    ):
        time(0.f),
        sheet(sheet),
        frame_rects(frame_rects),
        frame_origins(frame_origins),
        fps(fps)
    {}

    Animation(
        float fps,
        sf::Sprite* sheet,
        v2i size,
        v2f origin
    ): 
        time(0.f),
        sheet(sheet){

        const sf::IntRect &sheet_rect = sheet->getTextureRect();

        int w = sheet_rect.size.x/size.x, h = sheet_rect.size.y/size.y;

        frame_origins.push_back(origin);

        // std::cout << sheet_rect.size.x << ' ' << size.x << '\n';

        for(int yi = 0; yi < h; yi++){
            for(int xi = 0; xi < w; xi++){
                frame_rects.push_back(sf::IntRect(v2i(xi*size.x,yi*size.y),size));
            }
        }

        this->fps = fps;
    }

    inline void tickFrame(float dt){
        if(playing)
            time += dt*fps;
    }

    inline void setFrame(int fr){
        time = fr;
    }

    inline void play(){
        playing = true;
    }

    inline void pause(){
        playing = false;
    }

    inline void stop(){
        setFrame(0);
        pause();
    }

    inline void reset(){
        setFrame(0);
        play();
    }

    const v2f getOrigin(){
        if(frame_origins.empty())return v2f();
        return frame_origins[(size_t)time%frame_origins.size()];
    }

    const sf::IntRect &getRect(){
        if(frame_rects.empty())return sheet->getTextureRect();
        return frame_rects[(size_t)time%frame_rects.size()];
    }
};


struct Sprite : Spatial{
    
    vector<Animation> anim;

    size_t current_animation_index = -1;
    // Depth value used to specify the order of drawn sprites.
    int z;

    // Empty constructor.
    Sprite();

    Sprite(
        string name,
        v2f pos, 
        float angle = 0.f, v2f scale = {1.f,1.f},
        Animation anim = Animation(),
        int z = 0,
        sf::Color debug_color = sf::Color::White
    );

    virtual StructId getStructId() override;

    const Animation &getCurrAnimation(){
        if(anim.empty()){
            cout << "No animations are present the hell are you trying to do (Animation::getCurrentAnimation)";
            throw std::exception();
        }
        return anim[current_animation_index];
    }

    void setCurrAnimation(size_t animation_index, bool reset_previous = false, bool reset_new = false){
        if(reset_previous){
            if(anim.empty())
                return;
            anim[current_animation_index].reset();
        }
        if(reset_new){
            anim[animation_index].reset();
        }
        current_animation_index = animation_index;
    }

    // virtual void drawDebug() override;
    virtual void draw() override{
        auto a = getCurrAnimation();

        Spatial::draw();
        if(!a.sheet)return;

        cout << "calling viewport::draw...\n";

        viewport::draw(
            a.sheet, 0, 0, z, 
            getGlobalTransform().translate(-a.getOrigin()), 
            a.getRect()
        );
        cout << "success\n";
    }

    // virtual void process() override;

    virtual void reader(fstream &stream) override{
        Spatial::reader(stream);

        z = fstreamRead<int>(stream);

        size_t anims_count = fstreamRead<size_t>(stream);
        anim = vector<Animation>(anims_count);
        for(size_t i = 0; i < anims_count; i++){
            Animation &a = anim[i];

            string sprite_name = fstreamRead(stream);
            a.sheet = &getSprite(sprite_name);

            a.fps = fstreamRead<float>(stream);

            size_t frames_count = fstreamRead<size_t>(stream);

            a.frame_origins = vector<v2f>(frames_count);
            a.frame_rects = vector<sf::IntRect>(frames_count);

            for(size_t i = 0; i < frames_count; i++){
                a.frame_rects[i].position = fstreamReadV2i(stream);
                a.frame_rects[i].size = fstreamReadV2i(stream);
                a.frame_origins[i] = fstreamReadV2f(stream);
            }
        }
    }
    virtual void writer(fstream &stream) override{
        Spatial::writer(stream);

        fstreamWrite<int>(stream, z);

        size_t anims_count = anim.size();
        fstreamWrite(stream, anims_count);

        for(auto &a: anim){
            string sprite_name = assets::getSpriteName(*a.sheet);
            fstreamWrite(stream, sprite_name);

            fstreamWrite(stream, a.fps);

            size_t frames_count = a.frame_rects.size();
            fstreamWrite(stream, frames_count);

            for(size_t i = 0; i < frames_count; i++){
                fstreamWriteV2i(stream, a.frame_rects[i].position);
                fstreamWriteV2i(stream, a.frame_rects[i].size);
                fstreamWriteV2f(stream, a.frame_origins[i]);
            }
        }

        
    }
    
    // virtual void printInfo(std::ostream& s) override;
    
    // Copy tree.
    // Make sure the node in pointer is a spatial.
    virtual void copy(weak_ptr<Node> node) override{
        Spatial::copy(node);
        if(auto n = node.lock()){
            auto sprite = dynamic_cast<Sprite*>(n.get());
            if(sprite){
                z = sprite->z;
                anim = sprite->anim;
            }
            else{
                cout << "Sprite NOT constructed due to the pointer not being Sprite.\n";
            }
        }
    }
};

#endif