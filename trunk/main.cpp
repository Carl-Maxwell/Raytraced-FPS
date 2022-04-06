
// Urbarack 3D, a 2.5D video game

// Written by Carl Maxwell.
// Based on the tutorial series by "the Cherno" on youtube
// https://www.youtube.com/watch?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <map>

#include <fstream>
#include <sstream>

#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h>

//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "glDebugMessageCallback.h"

// Maxwell Libraries

#include "mvec3.h"
#include "mmath.h"
#include "mtypedefs.h"
#include "marray.h"

//
// Includes that are actually part of this codebase (not external libraries)
//

#include "rng.h"
#include "noah.h"
#include "uid.h"
#include "application.h"
#include "texture.h"
#include "shader.h"
#include "renderer.h"
#include "dlog.h"
#include "actor.h"

#include "entity.h"
#include "component.h"
#include "components/transform_component.h"
#include "components/sphere_component.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// static void glfw_error_callback(int error, const char* description)
// {
//     fprintf(stderr, "Glfw Error %d: %s\n", error, description);
// }

//
// Input handling
//

float player_turn_yaw = 0.0f;
float player_turn_pitch = 0.0f;
float player_forward_move = 0.0f;
float player_strafe_move = 0.0f;
float player_speed = 0.01f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);

  } else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    player_strafe_move += -1.00f;
  } else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
    player_strafe_move +=  1.00f;
  } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    player_strafe_move += 1.00f;
  } else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
    player_strafe_move -= 1.00f;

  } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    player_forward_move +=  1.00f;
  } else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
    player_forward_move -=  1.00f;
  } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    player_forward_move += -1.00f;
  } else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
    player_forward_move +=  1.00f;

  } else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
    player_speed = 0.25f;
  } else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
    player_speed = 0.01f;
  }
}

//
// Scene
//

struct Scene{
  std::vector<Entity*> entities;
  std::vector<TransformComponent*> transformComponents;
  std::vector<SphereComponent*> sphereComponents;
  std::map<Uid, Entity*> entity_by_uid;
  Entity* getEntity(Uid id) {
    if (entity_by_uid.count(id) == 0) {
      std::cout << "Error! No entity " << id.to_english() << "\n";
      return nullptr;
    }

    auto iter = entity_by_uid.find(id);
    if (iter == entity_by_uid.end()) {
      std::cout << "No such entity " << id.to_english() << "\n";
      return nullptr;
    } 

    // return nullptr;

    Entity* output = entity_by_uid[id];

    if (output->m_entity_id.id != id.id) {
      std::cout << "Error! getEntity() fetched the wrong entity! wanted \n\t" 
        << id.to_english() << "(index[" << getEntityIndex(id) << "])" << " \tbut got \n\t" 
        << output->m_entity_id.to_english() << " (" << getEntityIndex(output->m_entity_id) << ")\n";
      return nullptr;
    }

    return entity_by_uid[id];
  }
  Entity* getSphere(int target_i) {
    // TODO this could just start at actual_i = target_i since it can't be less than that
    int sphere_i = 0;
    for(int actual_i=0; actual_i < entities.size(); actual_i++) {
      if (entities[actual_i]->getComponent<SphereComponent>("SphereComponent")) {
        if (sphere_i == target_i) {
          return entities[actual_i];
        }

        sphere_i++;
      }
    }
  }
  u64 getEntityIndex(Uid id) {
    for (int i = 0; i < entities.size(); i++) {
      if (entities[i]->m_entity_id.id == id.id) {
        return i;
      }
    }
    return u64(-1);
  }

} scene;

Entity* createSphere(m::vec3 position, f32 radius) {
  scene.transformComponents.push_back(new TransformComponent(position));
  scene.sphereComponents.push_back(new SphereComponent(radius));

  Entity* nSphere = new Entity;

  scene.entities.push_back(nSphere);

  Uid id = nSphere->m_entity_id;

  if (scene.entity_by_uid.count(id) == 0) {
    // std::cout << "adding " << id.to_english() << " to entity_by_id at index[" << scene.entities.size()-1 << "]\n";
  }

  // scene.entity_by_uid[nSphere->m_entity_id.id] = nSphere;
  auto [iter, success] = scene.entity_by_uid.insert({ nSphere->m_entity_id, nSphere });

  auto back = scene.entity_by_uid.begin();
  auto next = scene.entity_by_uid.begin();
  next++;

  for (; next != scene.entity_by_uid.end(); next++) {
    // std::cout << back->first << ":" << back->second->m_entity_id.to_english() << "\n";
    back++;
  }

  // std::cout << "inserted: " << nSphere->m_entity_id.id << ": " << nSphere->m_entity_id.to_english() << "\n";
  // std::cout << "which is: " << back->first.to_english() << ": "<< back->second->m_entity_id.to_english() << "\n";

  if (!success) {
    std::cout << "Insertion failed!\n";
  }

  scene.getEntity(id);

  nSphere->pushComponent<TransformComponent>( scene.transformComponents.back() );
  nSphere->pushComponent<SphereComponent   >( scene.sphereComponents.back() );

  return nSphere;
}

//
// Entry point
//

int main() {
  Print::heading("Initializing");

  Application::the();
  RNG::the().setup_rng();
  Noah::the();

  std::cout << "Starting Urbarak 3D...\n";

  //
  // Map
  //

  u32* map; // this is actually a bool

  map = new u32[100*100];

  for (int x = 0; x < 100; x++) {
    for (int y = 0; y < 100; y++) {
      *(map + x * 100 + y) = false;
    }
  }

  *(map + 3 * 100 + 0) = true; // the only wall

  //---------------------------------------------------------------------------
  // Actors
  //---------------------------------------------------------------------------

  Print::heading("Setting up actors");

  //
  // Setup the spheres
  //

  const int sphere_count = 100;
  const int sphere_max_radius = 2.0f;
  Actor sphere[sphere_count];

  // TODO is there some way to have sphere_uniform setup without setting the memory every frame?
  m::vec4* sphere_uniform = new m::vec4[sphere_count];

  Print::heading2("Creating spheres");

  createSphere(m::vec3(0, -100.0f, 0), 100.0f);

  for(int i = 0; i < sphere_count; i += 1) {
    createSphere(
      m::vec3( // position
        RNG::the().randf(-20, 20),
        RNG::the().randf(-3, 3),
        RNG::the().randf(-20, 20)
      ),
      RNG::the().randf(0.3f, sphere_max_radius)
    );
  }

  for(int i = 0; i < sphere_count; i += 1) {
    Entity* theSphere = scene.getSphere(i);
    scene.getEntity(theSphere->m_entity_id);
  }

  m::vec3 cameraPosition = {20.0f, 1.0f, 1.0f};
  float camera_yaw = 0;
  float camera_pitch = 0;

  //
  // Billboard Actors
  //
  Print::heading2("Setting up billboard actors");

  const i32 billboard_count = 4;

  Actor billboards[billboard_count];

  m::vec3* billboardPositions = new m::vec3[billboard_count];// {
  //   {-22.0f, 1.0f, -22.0f},
  //   {22.0f, 1.0f, 22.0f},
  //   {-20.0f, -1.0f, 20.0f},
  //   {20.0f, 1.0f, -20.0f}
  // };

  m::vec3* billboardTargetPositions = new m::vec3[billboard_count];

  for (int i = 0; i < billboard_count; i++) {
    billboardTargetPositions[i] = billboards[i].m_position * -1;
  }

  // summarize actor initialization

  Print::heading2("Initialized " + std::to_string(scene.entities.size()) + " actors into the scene");

  //---------------------------------------------------------------------------
  // Initialize Graphics
  //---------------------------------------------------------------------------

  Print::heading("Initializing graphics");

  //
  // Initialize glfw
  //

  GLFWwindow* window;

  if (!glfwInit())
    return -1;

  int WIDTH = 1280, HEIGHT = 720;
  // int WIDTH = 1920, HEIGHT = 1080;

  // glfwGetPrimaryMonitor() param before last

  window = glfwCreateWindow(WIDTH, HEIGHT, "Urbarak 3d", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  Application::the().window = window;

  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  //
  // Initialize glew
  //

  {
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK)
      fprintf(stderr, "glew init Error: %s\n", glewGetErrorString(glew_error));
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    std::cout << glGetString(GL_VERSION) << "\n";
  }

  //
  // Enable opengl error handling
  //

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(GLDebugMessageCallback, NULL);

  //
  // Create triangle geometry
  //

  float positions[] = {
    -1.00f, -1.00f,
     1.00f,  1.00f,
     1.00f, -1.00f,

    -1.00f, -1.00f,
    -1.00f,  1.00f,
     1.00f,  1.00f
  };

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  // Initialize shaders

  Print::heading2("Initialize shaders");

  Shader shader("resources/shaders/primaryshader.glsl");
  shader.Bind();

  //
  // Load Images
  //

  Print::heading2("Loading images");

  Texture badGuyImage = Texture("resources/images/badguy.png");
  badGuyImage.bind();
  shader.SetUniform1i("u_texture", 0);

  //---------------------------------------------------------------------------
  // Main loop
  //---------------------------------------------------------------------------

  Print::heading("Main loop");

  // setup mouse position variables

  double mouse_xpos, mouse_ypos, mouse_xpos_old, mouse_ypos_old;
  glfwGetCursorPos(window, &mouse_xpos, &mouse_ypos);
  mouse_xpos_old = mouse_xpos;
  mouse_ypos_old = mouse_ypos;
  float mouse_xdelta, mouse_ydelta;

  Log::the().push("seed: " + std::to_string((unsigned long)Application::the().startTime * 1'000'000));

  std::map<u64, int> rngtest;

  while (!glfwWindowShouldClose(window))
  {
    Renderer::the().Clear();

    Renderer::the().Draw(shader);

    Application::the().time = Application::the().getTimestamp();
    Log::the().tick();

    // camera_yaw += player_turn_yaw;
    // camera_pitch += player_turn_pitch;

    // handle mouse input
    glfwGetCursorPos(window, &mouse_xpos, &mouse_ypos);

    mouse_xdelta = float(mouse_xpos - mouse_xpos_old);
    mouse_ydelta = float(mouse_ypos - mouse_ypos_old);

    camera_yaw += mouse_xdelta/10'000.0f;

    float npitch = camera_pitch + mouse_ydelta / 10'000.0f;

    if (npitch > -1.45 && npitch < 1.45) {
      camera_pitch += mouse_ydelta / 10'000.0f;

      // if you go past straight up or straight down on pitch it inverts the camera or something
      // so 1.5 radians is about ~90 degrees I guess
    }

    mouse_xpos_old = mouse_xpos;
    mouse_ypos_old = mouse_ypos;

    while (camera_yaw > m::degrees_to_radians(360)) {
      camera_yaw -= m::degrees_to_radians(360);
    }
    while (camera_yaw < -m::degrees_to_radians(360)) {
      camera_yaw += m::degrees_to_radians(360);
    }
    // TODO are there any edge cases with these while loops?

    //
    // calculate camera position, player movement
    //

    {
      float yaw, pitch;
      yaw = camera_yaw;
      pitch = 0.0f;

      // TODO multiply by frame time

      // forward movement

      float distance = player_forward_move * player_speed;
      cameraPosition.x += -distance * cos(yaw);
      cameraPosition.z += -distance * sin(yaw);

      // strafing

      distance = player_strafe_move * player_speed;
      cameraPosition.x +=  distance * sin(yaw);
      cameraPosition.z += -distance * cos(yaw);
    }

    //
    // send variables off to shader
    //

    // TODO shouldn't have to setup the uniforms like this.
    // The data should just be laid out the right way in memory.

    for (int i=0; i < sphere_count; i++) {
      Entity* theSphere = scene.getEntity(scene.sphereComponents[i]->m_entity_id);

      TransformComponent* transf_comp = theSphere->getComponent<TransformComponent>("TransformComponent");
      SphereComponent*    sphere_comp = theSphere->getComponent<SphereComponent   >("SphereComponent");

      sphere_uniform[i]   = theSphere->getComponent<TransformComponent>("TransformComponent")->position;
      sphere_uniform[i].w = theSphere->getComponent<SphereComponent>("SphereComponent")->radius;
    }

    for (int i=0; i < billboard_count; i++) {
      billboardPositions[i] = billboards[i].m_position;
    }

    shader.SetUniform1f("time", float(Application::the().time) );
    shader.SetUniform2f("windowSize", float(WIDTH), float(HEIGHT));
    shader.SetUniform1uiv("gameMap", 100*100, map);
    shader.SetUniform("sphere", sphere_count, sphere_uniform);
    shader.SetUniform1f("cameraYaw", &camera_yaw);
    shader.SetUniform1f("cameraPitch", &camera_pitch);
    shader.SetUniform("cameraPosition", 1, &cameraPosition);
    shader.SetUniform("billboardPositions", billboard_count, billboardPositions);
    shader.SetUniform1i("billboard_count", billboard_count);

    //
    // Move actors
    //

    // Move spheres

    float movescale = 0.03f;

    for (int i=0; i < sphere_count; i++) {
      Entity* theSphere = scene.getEntity(scene.sphereComponents[i]->m_entity_id);
      float radius = theSphere->getComponent<SphereComponent>("SphereComponent")->radius;

      if (radius > sphere_max_radius) {
        continue;
      }

      float energy = (sphere_max_radius-radius);
      energy = energy < 0 ? 0 : energy;

      theSphere->getComponent<TransformComponent>("TransformComponent")->position += RNG::the().randvec3(-1, 1) * energy;

      for (int i2 = 0; i2 < sphere_count; i2++) {
        Entity* sphere2 = scene.getEntity(scene.sphereComponents[i2]->m_entity_id);
        float radius2 = sphere2->getComponent<SphereComponent>("SphereComponent")->radius;
        float big_radius = radius + radius2;

        if (i == i2) {
          continue;
        }

        m::vec3 position  = theSphere ->getComponent<TransformComponent>("TransformComponent")->position;
        m::vec3 position2 = sphere2   ->getComponent<TransformComponent>("TransformComponent")->position;

        if ((position - position2).length() < big_radius) {
          if (radius < radius2) {
            position  = position  + (position  - position2).norm() * 0.1;
          } else {
            position2 = position2 + (position2 - position ).norm() * 0.1;
          }
        }

        if ((position - cameraPosition).length() < 3) {
          position = position + (position - cameraPosition).norm() * player_speed;
        }

        theSphere ->getComponent<TransformComponent>("TransformComponent")->position = position;
        sphere2   ->getComponent<TransformComponent>("TransformComponent")->position = position2;
      }
    }

    // Move Billboard Actors

    {
      for (int i = 0; i < billboard_count; i++) {
        m::vec3 distance = billboardTargetPositions[i] - billboards[i].m_position;
        m::vec3 direction = (distance).norm();

        billboards[i].m_position = billboards[i].m_position + direction * 0.1f;

        if (distance.length() < 3.0f) {
          billboardTargetPositions[i].x = RNG::the().randf(-40, 40);
          billboardTargetPositions[i].z = RNG::the().randf(-40, 40);
        }

      }
    }

    glfwSwapBuffers(window); // swap front and back buffers

    glfwPollEvents();
  }

  glfwTerminate();

  delete[] billboardTargetPositions;
  delete[] billboardPositions;
  delete[] map;

  return 0;
}
