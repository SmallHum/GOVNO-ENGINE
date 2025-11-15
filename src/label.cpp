#include <structs/label.h>
#include <core/viewport.h>
#include <struct_loader.h>

Label::Label():
    Spatial(),
    font(&getFont("main:font_default")),
    text("Text here."), 
    z(0)
{
    name = "Label";
}

Label::Label(string name,
        v2f pos, float angle, v2f scale,
        string text,
        int z,
        GVEFont &font,
        sf::Color debug_color):

        Spatial(name, pos, angle, scale, debug_color),
        font(&font),
        text(text),
        z(z){

        }

StructId Label::getStructId(){
    return StructId::Label;
}

void Label::draw(){
    v2f curr_pos;
    for(auto &i : text){
        if(i != '\n'){
            viewport::draw(font->getGlyph(i),0,0,z, getGlobalTransform().translate(curr_pos));
            curr_pos.x += font->char_size.x;
            // cout << i;
            continue;
        }
        curr_pos = {0, curr_pos.y + font->char_size.y};
        // cout << '\n'; 
    }
    Spatial::draw();
}

// void Label::drawDebug(){
//     Spatial::drawDebug();
// }

void Label::reader(fstream &stream){
    Spatial::reader(stream);

    z = fstreamRead<int>(stream);
    text = fstreamRead(stream);

    // Yup. That's gonna be asset management.
    font = &getFont(fstreamRead(stream));
}
void Label::writer(fstream &stream){
    Spatial::writer(stream);

    fstreamWrite<int>(stream, z);
    fstreamWrite(stream, text);

    string font_name = assets::getFontName(*font);
    fstreamWrite(stream, font_name);
}

void Label::copy(weak_ptr<Node> node){
    Spatial::copy(node);
    if(auto n = node.lock()){
        auto label = dynamic_cast<Label*>(n.get());
        if(label){
            text = label->text;
            font = label->font;
            z = label->z;
        }
        else{
            cout << "Label NOT constructed due to the pointer not being Label.\n";
        }
    }
}