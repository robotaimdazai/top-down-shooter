#include "scene_main.h"
#include "imgui.h"
#include "../engine/resource_manager.h"
#include "../engine/ecs/ecs.h"
#include "../engine/ecs/components/animator.h"
#include "../engine/ecs/components/camera.h"
#include "../engine/ecs/components/player_input.h"
#include "../engine/ecs/components/sprite.h"
#include "../engine/ecs/components/transform.h"
#include "../engine/ecs/systems/renderer_sprite.h"
#include "../engine/ecs/systems/system_animation.h"
#include "../engine/ecs/systems/system_camera.h"
#include "../engine/ecs/systems/system_player_input.h"

ecs g_ecs;
std::shared_ptr<system_camera> g_camera_system;
std::shared_ptr<renderer_sprite> g_sprite_system;
std::shared_ptr<system_player_input> g_player_movement;
std::shared_ptr<system_animation> g_animation_system;;


//entities/gameobjects  in scene
entity g_player;
entity g_camera;

auto scene_main::set_game(game* game) -> void
{
    m_game_ = game;
    g_ecs.init();
    
    //register components first
    g_ecs.register_component<sprite>();
    g_ecs.register_component<transform>();
    g_ecs.register_component<camera>();
    g_ecs.register_component<player_input>();
    g_ecs.register_component<animator>();
    
    //loading shader for sprite renderer
    resource_manager::load_shader(SHADER_SPRITE_PATH, SHADER_SPRITE_NAME);
    auto sprite_shader =resource_manager::get_shader(SHADER_SPRITE_NAME);
    //registering sprite system
    g_sprite_system = g_ecs.register_system<renderer_sprite>(sprite_shader);
    signature sprite_system_signature;
    sprite_system_signature.set(g_ecs.get_component_type<sprite>());
    sprite_system_signature.set(g_ecs.get_component_type<transform>());
    g_ecs.set_system_signature<renderer_sprite>(sprite_system_signature);
    

    //registering camera system
    g_camera_system = g_ecs.register_system<system_camera>();
    signature camera_signature;
    camera_signature.set(g_ecs.get_component_type<transform>());
    camera_signature.set(g_ecs.get_component_type<camera>());
    g_ecs.set_system_signature<system_camera>(camera_signature);

    //registering player input system
    g_player_movement = g_ecs.register_system<system_player_input>();
    signature movement_signature;
    movement_signature.set(g_ecs.get_component_type<transform>());
    movement_signature.set(g_ecs.get_component_type<player_input>());
    movement_signature.set(g_ecs.get_component_type<animator>());
    g_ecs.set_system_signature<system_player_input>(movement_signature);

    //registering animation system
    g_animation_system = g_ecs.register_system<system_animation>();
    signature animation_signature;
    animation_signature.set(g_ecs.get_component_type<sprite>());
    animation_signature.set(g_ecs.get_component_type<animator>());
    g_ecs.set_system_signature<system_animation>(animation_signature);
    

}

auto scene_main::load() -> void
{
    //load all resources
    resource_manager::load_texture("assets/textures/dazai-sheet.png","dazai");
    
    //player
    g_player = g_ecs.add_entity();
    g_ecs.add_component<transform>(g_player);
    g_ecs.add_component<sprite>(g_player);
    g_ecs.add_component<player_input>(g_player);
    auto& player_sprite =g_ecs.get_component<sprite>(g_player);
    player_sprite.texture_id ="dazai";
    player_sprite.size ={64,64};
    g_ecs.add_component<animator>(g_player);
    player_sprite.is_animated = true;
    auto& player_animator = g_ecs.get_component<animator>(g_player);
    player_animator.animations.emplace("idle",animation(0,{32,31},13,8));
    player_animator.animations.emplace("run",animation(1,{32,32},8,20));
    player_animator.animations.emplace("attack",animation(4,{32,32},10,30,true));
    player_animator.change_animation("idle");
    
    //create camera
    g_camera = g_ecs.add_entity();
    g_ecs.add_component<transform>(g_camera);
    g_ecs.add_component<camera>(g_camera);
    
}

auto scene_main::update(const float delta_time) -> void
{
    g_camera_system->update();
    g_player_movement->update(delta_time);
    g_animation_system->update(delta_time);
}

auto scene_main::render() -> void
{
    //render all entities
    g_sprite_system->render();
}

auto scene_main::handle_event(const input_state& input_state) -> void
{
    g_player_movement->handle_event(input_state);
}


auto scene_main::pause() -> void
{
    
}
auto scene_main::resume() -> void
{
    
}

auto scene_main::clean() -> void
{
   
}
float x=0.0f;
float y=0.0f;
auto scene_main::on_gui() -> void
{
    ImGui::Begin("Debug");
    ImGui::Text("Camera");
    ImGui::SliderFloat3("position",&g_ecs.get_component<transform>(g_camera).position.x,-1280.0f,1280.0f);
    ImGui::SliderFloat("zoom",&g_ecs.get_component<camera>(g_camera).zoom,0,2.0f);
    ImGui::End();
}




