#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
void main() {
  gl_Position = position;
};

#shader fragment
#version 400 core
layout(location = 0) out vec4 color;

in vec4 gl_FragCoord;

uniform float time;
uniform vec2 windowSize;
uniform uint gameMap[100]; // apparently there's a limit on the max size of arrays?
uniform vec4 sphere[100];
uniform float cameraYaw;
uniform float cameraPitch;
uniform vec3 cameraPosition;
uniform int billboard_count = 3;
uniform vec3 billboardPosition[100];

uniform sampler2D u_texture;

#define pi 3.1415926535897932384626433832795

float degrees_to_radians(float degrees) {
  return degrees * pi / 180.0f;
}

float hit_sphere(vec3 center, double radius, vec3 r) {
  vec3 oc = cameraPosition - center;
  double a = dot(r, r);
  double b = 2.0 * dot(oc, r);
  double c = dot(oc, oc) - radius*radius;
  double discriminant = b*b - 4*a*c;
  // return (discriminant > 0);
  if (discriminant < 0) {
       return -1.0f;
   } else {
       return float( (-b - sqrt(discriminant) ) / (2.0*a) );
   }
}

// vec3 msign(vec3 vsign, vec3 v) {
//   // v.x = v.x / abs(v.x);
//   // v.y = v.y / abs(v.y);
//   // v.z = v.z / abs(v.z);

//   vec3 temp;

//   vsign.x = v.x < 0.0f ? -1.0f : 1.0f;
//   vsign.y = v.y < 0.0f ? -1.0f : 1.0f;
//   vsign.z = v.z < 0.0f ? -1.0f : 1.0f;

//   v.x = abs(v.x) * vsign.x;
//   v.y = abs(v.y) * vsign.y;
//   v.z = abs(v.z) * vsign.z;

//   return temp;
// }

void main() {
  //
  // Calculate camera perspective
  //

  float vfov = 90.0f;
  float aspect_ratio = 16.0f / 9.0f;

  float theta = degrees_to_radians(vfov);
  float h = tan(theta/2.0f);
  float viewport_height = 2.0f * h;
  float viewport_width = aspect_ratio * viewport_height;

  float pitch = cameraPitch;

  vec3 w;
  w.x = cos(cameraYaw)*cos(pitch);
  w.y = sin(pitch);
  w.z = sin(cameraYaw)*cos(pitch);

  vec3 lookfrom = cameraPosition;
  // vec3 lookat;
  vec3 vup = vec3(0, 1, 0);
  vec3 cameraUpVector = vup;
  // vec3 w = normalize(lookfrom - lookat);
  vec3 screenx = normalize(cross(vup, w));
  vec3 screeny = cross(w, screenx);

  vec3 origin = cameraPosition;
  vec3 horizontal = viewport_width * screenx;
  vec3 vertical = viewport_height * screeny;
  vec3 lower_left_corner = origin - horizontal/2 - vertical/2 - w;

  // double focal_length = 1.0;

  // vec3 origin = vec3(0, 0, 0);
  // vec3 horizontal = vec3(viewport_width, 0, 0);
  // vec3 vertical = vec3(0, viewport_height, 0);
  // vec3 lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

  float u = gl_FragCoord.x / windowSize.x;
  float v = gl_FragCoord.y / windowSize.y;

  vec3 ray_direction = lower_left_corner + u*horizontal + v*vertical - origin;

  // ray direction = lower_left_corner +
  // screen_x*camera_horizontal +
  // screen_y*camera_vertical -
  // camera_position

  //
  // Calculate noise
  //

  float r = fract(sin(dot((gl_FragCoord.xy + windowSize.xy/2) * sin(time/10000), vec2(12.9898, 78.233))) * 43758.5453);
  float g = fract(sin(dot((gl_FragCoord.xy + windowSize.xy/2) * sin(time*0.9/10000), vec2(78.233, 12.9898))) * 43758.5453);
  //length(gl_FragCoord.xy - windowSize.xy/2)/length(windowSize);//length((gl_FragCoord.xy) - vec2(1.f, 0.5f));
  float b = fract(sin(dot((gl_FragCoord.xy + windowSize.xy/2) * sin(time*1.1/10000), vec2(78.233*2, 12.9898*2))) * 43758.5453);

  // round(length(gl_FragCoord.xy - windowSize.xy/2 )/length(windowSize/2) -- draw circle

  vec3 noise = vec3(r, g, b);

  //----------------------------------------------------------------------------
  // Draw Map
  //----------------------------------------------------------------------------

  // player is at 0, 0
  // player is facing +1, +0

  // for a 90 degree view cone the sides will be 45 degrees off +1, +0.

  float depth = 0;
  bool found = false;

  // if (gl_FragCoord.x > windowSize.x/2) {
  //   if (gameMap[ 999 ] == 1u)  {
  //
  //   }
  //
  //   for (int i = 0; !found && i < 100; i++) {
  //     if (gameMap[ 0 ] != 0u) { // i * 100 + 0
  //       depth += i;
  //       found = true;
  //     }
  //   }
  // }

  // gl_FragCoord.xy + windowSize.xy/2;

  //color = vec4(r, g, b, 1.0);

  //----------------------------------------------------------------------------
  // Draw Billboards
  //----------------------------------------------------------------------------

  // TODO should we use capsule collision for billboards?

  ray_direction;
  vec3 nRayDirection = normalize(ray_direction);
  cameraPosition;
  cameraUpVector;

  vec3 startingCameraPosition = vec3(20.0f, 1.0f, 1.0f);

  for (int i = 0; i < billboard_count; i++) {
    vec3 billboardSize = vec3(1, 2, 1);
    // vec3 billboardPosition = false ? vec3(0, 0, 0) : startingCameraPosition + vec3(15, 0, 0);
    vec3 billboardNormal = normalize(billboardPosition[i] - cameraPosition);
    // vec3 cameraRightVector = vec3(
    //   sin(nRayDirection.x),
    //   nRayDirection.y,
    //   cos(nRayDirection.z)
    // );

    // is ray_direction+camera_position pointing at the billboard?

    // dot(cameraPosition + ray_direction, billboardPosition);

    // TODO project the ray onto an infinite plane located where the billboard is
      // we want to get the point along the plane where the ray intersects
      // the plane would always be facing right towards the camera
    // TODO do a 2d check to see if the point on the infinite plane is inside the billboard

    // this website says the equation is ( https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection )
    // float rayLength =
    //   dot( (cameraPosition - billboardPosition[i]), billboardNormal )
    //   /
    //   dot(nRayDirection, billboardNormal)
    // ;

    // And that first term seems like it should be able to be simplified
    // It seems like it's always -1 * length(distance_to_billboard) or 1 * length(distance_to_billboard)
    // so

    // float rayLength = -1.0f * abs(length(cameraPosition - billboardPosition[i])) / dot(billboardNormal, ray_direction);

    // actually it seems like the order of the vectors doesn't matter, so the topside becomes dot (v, normalize(v)) which is just length(v)

    // color = vec4(1.0f - dot(normalize(ray_direction), billboardNormal));
    // return;

    float rayLength = length(billboardPosition[i] - cameraPosition) / dot(billboardNormal, normalize(ray_direction));

    // if (u < 0.5) {
    //   color = vec4(rayLength / 10);
    //   return;
    // }

    // display the xyz directions as rgb on half the screen

    // if (u > 0.5) {
    //   vec3 testColor = normalize(ray_direction);
    //   // testColor = abs(testColor);
    //   testColor = round(testColor*10)/10;
    //   color.xyz = testColor;
    //   return;
    // }

    if (dot(billboardNormal, normalize(ray_direction)) > 0) {
      vec3 rayPoint = cameraPosition + rayLength*normalize(ray_direction);
      vec3 planarPixel = rayPoint - billboardPosition[i];

      // vec3 rayInsideBillboard = step(billboardPosition - billboardSize, rayPoint);
      // rayInsideBillboard += step(billboardPosition + billboardSize, rayPoint);
      // bool isOnBillboard = 6 == rayInsideBillboard.x+rayInsideBillboard.y+rayInsideBillboard.z;

      // vec3 rayInsideBillboard = rayPoint - billboardPosition;
      // bool isOnBillboard = rayInsideBillboard.x > 0 && rayInsideBillboard.y > 0 && rayInsideBillboard.z > 0;
      //      isOnBillboard = rayInsideBillboard.x < billboardSize.x && rayInsideBillboard.y < billboardSize.y && rayInsideBillboard.z < billboardSize.z;

      // vec3 billBoardBounds = billboardPosition + billboardSize;

      // bool isOnBillboard = rayPoint.x > billboardPosition.x && rayPoint.y > billboardPosition.y && rayPoint.z > billboardPosition.z;
      //      isOnBillboard = rayPoint.x < billBoardBounds.x && rayPoint.y < billBoardBounds.y && rayPoint.z < billBoardBounds.z;

      // if (isOnBillboard) {
      //   color = vec4(1.0f);
      //   return;
      // } else {
      //   color = vec4(0.0f);
      //   return;
      // }

      // bool isOnBillboard = abs(planarPixel.x) < dist && abs(planarPixel.y) < dist && abs(planarPixel.z) < dist;

      // bool isOnBillboard = length(planarPixel) < 30.0f;

      {
        vec3 pointWithinBillboard = planarPixel/billboardSize;
        vec3 cameraRightVector = cross(nRayDirection, cameraUpVector);

        // TODO is cameraRightVector actually pointing right?
        // TODO the billboard image gets distorted at some angles

        vec2 texcoord = vec2(
          length(pointWithinBillboard*normalize(cameraRightVector)),
          length(pointWithinBillboard*normalize(cameraUpVector))
        );

        vec4 texColor = texture(u_texture, texcoord);

        // if (length(texcoord) < 10.0f) {
        //   color = texColor;
        //   return;
        // }

        if (texcoord.x > 0.0f && texcoord.x < 1.0f && texcoord.y > 0.0f && texcoord.y < 1.0f) {
          color = texColor;
          return;
        }

        color.w = 1.0f;
      }
    }
  }

  //---------------------------------------------------------------------------
  // Draw Spheres
  //---------------------------------------------------------------------------
  bool drawn = false;

  float t = 0;

  {
    bool found = false;
    vec4 foremostSphere;
    vec3 intersectionPoint;
    float final_t;
    for (int i = 0; i < sphere.length; i++) {
      t = hit_sphere(sphere[i].xyz, sphere[i].w, ray_direction);

      if (t > 0.0f) {
        if (!found) {
          foremostSphere = sphere[i];
          intersectionPoint = ray_direction*t;
          final_t = t;
          found = true;
        } else if (length(ray_direction*t) < length(intersectionPoint)) {
          foremostSphere = sphere[i];
          intersectionPoint = ray_direction*t;
          final_t = t;
        }
      }
    }
    if (found) {
      t = final_t;

      vec3 N = normalize((cameraPosition + t*ray_direction) - foremostSphere.xyz);
      vec3 thing2 = 0.5*vec3(N.x+1, N.y+1, N.z+1);
      color = vec4(thing2.x, thing2.y, thing2.z, 1.0);
      drawn = true;
      return;
    }
  }

  // draw background

  if (!drawn) {
    vec3 unit_direction = normalize(ray_direction);
    t = 0.5*(unit_direction.y + 1.0);
    vec3 thing = (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);

    // thing = (thing+noise)/2.0f;

    color = vec4(thing.x, thing.y, thing.z, 1.0);
  }
};
