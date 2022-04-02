
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

//
// Includes that are actually part of this codebase (not external libraries)
//

#include "application.h"
#include "texture.h"
#include "shader.h"
#include "renderer.h"
#include "dlog.h"
#include "actor.h"

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
// RNG
//

struct random{
  std::default_random_engine engine;
  std::uniform_int_distribution<unsigned long long> dist;
  void setup_rng() {
    // dist.seed((unsigned long)Application::the().startTime);
  }

  //
  // Random Integers
  //

  int die(int sides) {
    return die(1, sides);
  }
  int die(int n, int sides) {
    int sum = 0;
    for (int i=0; i < n; n++) {
      sum += (_rngul() % sides) + 1;
    }
    return sum;
  }
  int randi32(int max = std::numeric_limits<int>::max()) { // 0..max
    return _rngul() % max;
  }
  u64 _rng() {
    return dist(engine);
  }
  u64 _rngul() {
    return _rng();
    // u64 data = 0;

    // const u64 base = 2;
    // const u64 useful_bits = 32;

    // for (u64 bits = 0; bits < 64; bits += 32) {
    //   data |= (rand() & m::pow(base, useful_bits)) << bits;
    // }
    // return data;

    // TODO how random is this? Is it biased towards one thing or another?
  }

  //
  // random floats
  //

  float randf() {
    return float(_rngd());
  }
  float randf(float min, float max) { // min..max
    double range = max-min;
    return float( _rngd() * double(range) + double(min) );
  }
  // meant this to be f64, dunno if that's what double is?
  double _rngd() { // 0..1
    return (double( _rngul()%10'000'000'000'000'001 ) / 10'000'000'000'000'000.0f);

    // note the off by 1 that comes from modulus ( for example rand()%4 is 0..3,
      // so for 1d4 you'd need rand()%5 )
  }
} RNG;

//
// Entry point
//

int main() {
  Application::the();
  RNG.setup_rng();

  std::cout << "Starting Urbarak 3D...\n";

  // seed the RNG
  std::srand(unsigned(std::time(nullptr)));

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

  //
  // Setup the spheres
  //

  const int sphere_count = 100;
  const int sphere_max_radius = 2.0f;
  Actor sphere[sphere_count];

  // TODO is there some way to have sphere_uniform setup without setting the memory every frame?
  m::vec4* sphere_uniform = new m::vec4[sphere_count];

  for(int i = 0; i < sphere_count; i += 1) {
    sphere[i].m_pType = PrimitiveType::sphere;
    sphere[i].m_position = m::vec3(
      RNG.randf(-20, 20),
      RNG.randf(-3, 3),
      RNG.randf(-20, 20)
    );
    sphere[i].sphere()->radius = RNG.randf(0.3f, sphere_max_radius);
  }

  sphere[0].m_position.x = 0;
  sphere[0].m_position.y = -100.0f;
  sphere[0].m_position.z = 0;
  sphere[0].sphere()->radius = 100.0f;

  m::vec3 cameraPosition = {20.0f, 1.0f, 1.0f};
  float camera_yaw = 0;
  float camera_pitch = 0;

  //
  // Billboard Actors
  //

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

  //---------------------------------------------------------------------------
  // Initialize Graphics
  //---------------------------------------------------------------------------

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

  Shader shader("resources/shaders/primaryshader.glsl");
  shader.Bind();

  //
  // Load Images
  //

  Texture badGuyImage = Texture("resources/images/badguy.png");
  badGuyImage.bind();
  shader.SetUniform1i("u_texture", 0);

  //---------------------------------------------------------------------------
  // Main loop
  //---------------------------------------------------------------------------

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
      sphere_uniform[i] = sphere[i].m_position;

      sphere_uniform[i].w = sphere[i].sphere()->radius;
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
      float radius = sphere[ i ].sphere()->radius;

      if (radius > sphere_max_radius) {
        continue;
      }

      float energy = (sphere_max_radius-radius);
      energy = energy < 0 ? 0 : energy;

      sphere[ i ].m_position.x += movescale * ( RNG.randf(-1, 1) ) * energy;
      sphere[ i ].m_position.y += movescale * ( RNG.randf(-1, 1) ) * energy;
      sphere[ i ].m_position.z += movescale * ( RNG.randf(-1, 1) ) * energy;

      for (int i2 = 0; i2 < sphere_count; i2++) {
        float big_radius = sphere[i].sphere()->radius + sphere[i2].sphere()->radius;

        if (i == i2) {
          continue;
        }

        m::vec3 position  = sphere[i] .m_position;
        m::vec3 position2 = sphere[i2].m_position;

        if ((position - position2).length() < big_radius) {
          if (sphere[i].sphere()->radius < sphere[i2].sphere()->radius) {
            position  = position  + (position  - position2).norm() * 0.1;
          } else {
            position2 = position2 + (position2 - position ).norm() * 0.1;
          }
        }

        if ((position - cameraPosition).length() < 3) {
          position = position + (position - cameraPosition).norm() * player_speed;
        }

        sphere[i] .m_position = position;
        sphere[i2].m_position = position2;
      }
    }

    // Move Billboard Actors

    {
      for (int i = 0; i < billboard_count; i++) {
        m::vec3 distance = billboardTargetPositions[i] - billboards[i].m_position;
        m::vec3 direction = (distance).norm();

        billboards[i].m_position = billboards[i].m_position + direction * 0.1f;

        if (distance.length() < 3.0f) {
          billboardTargetPositions[i].x = RNG.randf(-40, 40);
          billboardTargetPositions[i].z = RNG.randf(-40, 40);
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
