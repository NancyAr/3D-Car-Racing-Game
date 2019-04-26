#include <application.hpp>
#include "directional_light_scene.hpp"
#include <mesh/mesh_utils.hpp>
#include <textures/texture_utils.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <Text.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
void initText2D(const char * texturePath, const char * texturePath2);
void cleanupText2D();
void printText2D(const char * text, int x, int y, int size, int index);
struct Particle {
    glm::vec3 Position;
    GLfloat Velocity;
    glm::vec4 Color;
    GLfloat Life;
  
    Particle() 
      : Position(0,0,0), Velocity(1.0f), Color(1.0f,0.0f,0.0f,0.0f), Life(1.0f) { }
};       
GLuint nr_particles = 500;
std::vector<Particle> particles;
GLuint amount = 10;
GLuint VBO;
GLuint VAO;

//GLuint firstUnusedParticle();
//void respawnParticle(Particle &particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
//void ParticleGeneratorDraw();
glm::vec3 ParticlePosition = {0,0,0};
void DirectionalLightScene::Initialize() {
    CarPosition = {0, 0, 0};
    float carRotation = 3.2;
    Box1Position =  {0,0,-5};
    Box2Position = {0.3,0,-10};
    Box3Position = {0.05,0,-15};
    LinePosition = {0,0,-15};

    shader = new Shader();
    shader->attach("assets/shaders/directional.vert", GL_VERTEX_SHADER);
    shader->attach("assets/shaders/directional.frag", GL_FRAGMENT_SHADER);
    shader->link();

    skyShader = new Shader();
    skyShader->attach("assets/shaders/sky.vert", GL_VERTEX_SHADER);
    skyShader->attach("assets/shaders/sky.frag", GL_FRAGMENT_SHADER);
    skyShader->link();
    
    particleshader = new Shader();
    particleshader->attach("assets/shaders/color.vert", GL_VERTEX_SHADER);
    particleshader->attach("assets/shaders/color.frag", GL_FRAGMENT_SHADER);
    particleshader->link();
   


    // Create this->amount default particle instances
    for (GLuint i = 0; i < amount; ++i)
        particles.push_back(Particle());

   
    plane = MeshUtils::Plane({0,0}, {1,1});
    ground = MeshUtils::Plane({0,0}, {5,5});
    box = MeshUtils::LoadObj("assets/models/Gift.obj");
    box2 = MeshUtils::LoadObj("assets/models/Gift.obj");
    box3 = MeshUtils::LoadObj("assets/models/Gift.obj");
    sky = MeshUtils::Box();
    car = MeshUtils::LoadObj("assets/models/car.obj");

    planetex[ALBEDO] = TextureUtils::Load2DTextureFromFile("assets/textures/Desert_Road_Diffuse.jpg");
    planetex[SPECULAR] = TextureUtils::Load2DTextureFromFile("assets/textures/Desert_Road_Specular.jpg");
    planetex[2] = TextureUtils::Load2DTextureFromFile("assets/textures/Desert_Road_Normal.jpg");
    
    boxtex[ALBEDO] = TextureUtils::Load2DTextureFromFile("assets/textures/Asphalt_col.jpg");
    boxtex[SPECULAR] = TextureUtils::Load2DTextureFromFile("assets/textures/Asphalt_spc.jpg");
    boxtex[ROUGHNESS] = TextureUtils::Load2DTextureFromFile("assets/textures/Asphalt_rgh.jpg");
    boxtex[AMBIENT_OCCLUSION] = TextureUtils::Load2DTextureFromFile("assets/textures/Suzanne_ao.jpg");
    boxtex[EMISSIVE] = TextureUtils::Load2DTextureFromFile("assets/textures/Asphalt_em.jpg");
    //boxtex[0] =TextureUtils::Load2DTextureFromFile("assets/textures/Asphalt_em.jpg");
    boxtex2[0] =TextureUtils::Load2DTextureFromFile("assets/textures/Asphalt_em.jpg");
    //boxtex3[0] =TextureUtils::Load2DTextureFromFile("assets/textures/Asphalt_em.jpg");
    parttext[0]=TextureUtils::Load2DTextureFromFile("assets/textures/particle.png");
    carTex[ALBEDO] = TextureUtils::Load2DTextureFromFile("assets/textures/car.png");
	carTex[SPECULAR] = TextureUtils::Load2DTextureFromFile("assets/textures/car.png");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   
    checkers[0] = TextureUtils::Load2DTextureFromFile("assets/textures/FinishLine.bmp");

    text[0] = TextureUtils::Load2DTextureFromFile("assets/textures/LCDish.tga");
    text[1] = TextureUtils::Load2DTextureFromFile("assets/textures/LogoFont.tga");

    camera = new Camera();
    glm::ivec2 windowSize = getApplication()->getWindowSize();
    camera->setupPerspective(glm::pi<float>()/2, (float)windowSize.x / windowSize.y, 0.1f, 1000.0f);
    camera->setUp({0, 1, 0});
    camera->setTarget({CarPosition.x, CarPosition.y, CarPosition.z});
    controller = new FlyCameraController(this, camera);
    controller->setYaw(-glm::half_pi<float>());
    controller->setPitch(-glm::quarter_pi<float>());
    controller->setPosition({CarPosition.x, CarPosition.y+0.5 , CarPosition.z+0.5});

    sunYaw = sunPitch = glm::quarter_pi<float>();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.88f,0.68f,0.15f,0.0f);
}
void DirectionalLightScene::ParticleGeneratorUpdate(double dt, GLuint newParticles, glm::vec3 offset)
{
    /*// Add new particles 
    for (GLuint i = 0; i < newParticles; ++i)
    {
        int unusedParticle = firstUnusedParticle();
        respawnParticle(particles[unusedParticle], offset);
    }
    // Update all particles
    for (GLuint i = 0; i < amount; ++i)
    {
        Particle &p = particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt; 
            p.Color.a -= dt * 2.5;
        }
    }*/
}
void DirectionalLightScene::ParticleGeneratorDraw()
{
    // Use additive blending to give it a 'glow' effect
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   /* particleshader->use();
    for (Particle particle : particles)
    {
        if (particle.Life > 0.0f)
        {
            particleshader->set("offset", particle.Position);
            glUniform4f(particleshader->getUniformLocation("color"), particle.Color.a, particle.Color.b, particle.Color.g,1.0f);
            //glm::mat4 part_mat = glm::scale(glm::mat4(), glm::vec3(0.09, 0.09, 0.09));
            //shader->set("M", part_mat);
            //glActiveTexture(GL_TEXTURE0);
            //parttext[0]->bind();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

        }
    }
    // Don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
}
//GLuint lastUsedParticle = 0;
/*GLuint firstUnusedParticle()
{
    // First search from last used particle, this will usually return almost instantly
    for (GLuint i = lastUsedParticle; i < amount; ++i){
        if (particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }*/
    // Otherwise, do a linear search
    /*for (GLuint i = 0; i < lastUsedParticle; ++i){
        if (particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void respawnParticle(Particle &particle, glm::vec3 offset)
{
    GLfloat random = ((rand() % 100) - 50) / 10.0f;
    //GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
    particle.Position = ParticlePosition + random + offset;
    particle.Color = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    particle.Life = 1.0f;
}*/

    double lastTime = glfwGetTime();
    double lastSecTime = glfwGetTime();
    double lastFrameTime = lastTime;
    int secsRemaining=5;
    int lastsec=secsRemaining;
    double currentTime = glfwGetTime();
    float deltaTime = (float)(currentTime - lastFrameTime);
    void computeRemaining()
    {
        double timeNow = glfwGetTime();
        if (timeNow - lastSecTime >= 1.0) //A second has passed
        {
            lastSecTime = timeNow;
            secsRemaining--;
        }
    }

 int time=0;
 int win = 0;
float pitch_speed = -10.0f, yaw_speed = 1.0f;
float car_speed = 0.1f;

void DirectionalLightScene::Update(double delta_time) {
    controller->update(delta_time);
    // Add new particles 
    /*for (GLuint i = 0; i < 3; ++i)
    {
        int unusedParticle = firstUnusedParticle();
        respawnParticle(particles[unusedParticle], ParticlePosition);
    }*/
    // Update all particles
   /* for (GLuint i = 0; i < amount; ++i)
    {
        Particle &p = particles[i];
        p.Life -= delta_time; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * delta_time; 
            p.Color.a -= delta_time * 2.5;
        }
    }*/
    Keyboard* kb = getKeyboard();
    if(kb->isPressed(GLFW_KEY_UP))
    {
        CarPosition.z -= (float)delta_time*car_speed;
        controller->setPosition({CarPosition.x, CarPosition.y+0.5 , CarPosition.z+0.5});
        if(trunc(CarPosition.z)==Box1Position.z && time == 0)
        {
            time = 1;
            if(CarPosition.x-Box1Position.x<=0.19 && CarPosition.x-Box1Position.x>=-0.19)
            {
                std::cout<<"\n Great Hit! Extra 2 seconds! \n";
                secsRemaining += 2;

                

            }    

        }
        if(trunc(CarPosition.z)==Box2Position.z  && (time == 0 || time==1))
        {
            time = 2;
            if(CarPosition.x-Box2Position.x<=0.19 && CarPosition.x-Box2Position.x>=-0.19)
             {  std::cout<<"\n Ouch! Mind your Head! \n";
                sunPitch = pitch_speed;
             }
           
        }
        if(trunc(CarPosition.z)==LinePosition.z  && win == 0)
        {
            if(secsRemaining>0)
            {
                std::cout<<"\n Congratulations! You Win!";
            }
            else
            std::cout<<"\n You lose. Better Luck next time!";
            
            win = 1;
        }
        if(time==2)
        {
            pitch_speed = 2.0f;
            sunPitch += (float)delta_time * pitch_speed;
        } 
        if(car_speed<4.0f)
            car_speed += 0.1f;
        
        if(lastsec-secsRemaining==1 && win==0)
        {
            std::cout<<" COUNTDOWN: ";
            std::cout<<secsRemaining;
        }
        //char secsRemText[256];
        lastsec=secsRemaining;
            //snprintf(minsRemText, sizeof(minsRemText), "%d", minsRemaining);
           //printText2D(minsRemText,130,500, 25, 0);
           //printText2D(":",1,1, 1, 0);
           //snprintf(secsRemText, sizeof(secsRemText), "%d", secsRemaining);
           //printText2D(secsRemText,10,10, 1, 0);
           computeRemaining();

    } 
    if(kb->isPressed(GLFW_KEY_DOWN)) 
    {
        CarPosition.z += (float)delta_time;
        car_speed=1.0f;
        controller->setPosition({CarPosition.x, CarPosition.y+0.5 , CarPosition.z+0.5});  
    }
    if(kb->isPressed(GLFW_KEY_LEFT))
    {  
        CarPosition.x -= (float)delta_time;
        controller->setPosition({CarPosition.x, CarPosition.y+0.5 , CarPosition.z+0.5});
    } 
    if(kb->isPressed(GLFW_KEY_RIGHT))
    {
        CarPosition.x += (float)delta_time;  
        controller->setPosition({CarPosition.x, CarPosition.y+0.5 , CarPosition.z+0.5});
    }


    if(kb->isPressed(GLFW_KEY_I)) sunPitch += (float)delta_time * pitch_speed;
    if(kb->isPressed(GLFW_KEY_K)) sunPitch -= (float)delta_time * pitch_speed;
    if(kb->isPressed(GLFW_KEY_L)) sunYaw += (float)delta_time * yaw_speed;
    if(kb->isPressed(GLFW_KEY_J)) sunYaw -= (float)delta_time * yaw_speed;
    
    if(sunPitch < -glm::half_pi<float>()) sunPitch = -glm::half_pi<float>();
    if(sunPitch > glm::half_pi<float>()) sunPitch = glm::half_pi<float>();
    sunYaw = glm::wrapAngle(sunYaw);
}

inline glm::vec3 getTimeOfDayMix(float sunPitch){
    sunPitch /= glm::half_pi<float>();
    if(sunPitch > 0){
        float noon = glm::smoothstep(0.0f, 0.5f, sunPitch);
        return {noon, 1.0f - noon, 0};
    } else {
        float dusk = glm::smoothstep(0.0f, 0.25f, -sunPitch);
        return {0, 1.0f - dusk, dusk};
    }
}

void DirectionalLightScene::Draw() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear colors and depth

    glm::mat4 VP = camera->getVPMatrix();
    glm::vec3 cam_pos = camera->getPosition();
    glm::vec3 sun_direction = glm::vec3(glm::cos(sunYaw), 0, glm::sin(sunYaw)) * glm::cos(sunPitch) + glm::vec3(0, glm::sin(sunPitch), 0);

    const glm::vec3 noonSkyColor = {0.53f, 0.81f, 0.98f};
    const glm::vec3 sunsetSkyColor = {0.99f, 0.37f, 0.33f};
    const glm::vec3 duskSkyColor = {0.04f, 0.05f, 0.19f};

    const glm::vec3 noonSunColor = {0.9f, 0.8f, 0.6f};
    const glm::vec3 sunsetSunColor = {0.8f, 0.6f, 0.4f};
    const glm::vec3 duskSunColor = {0.0f, 0.0f, 0.0f};
    
    glm::vec3 mix = getTimeOfDayMix(sunPitch);

    glm::vec3 skyColor = mix.x * noonSkyColor + mix.y * sunsetSkyColor + mix.z * duskSkyColor;
    glm::vec3 sunColor = mix.x * noonSunColor + mix.y * sunsetSunColor + mix.z * duskSunColor;

    shader->use();
    shader->set("VP", VP);
    shader->set("cam_pos", cam_pos);
    shader->set("light.color", sunColor);
    shader->set("light.direction", -sun_direction);
    shader->set("ambient", 0.5f*skyColor);

    shader->set("material.albedo", 0);
    shader->set("material.specular", 3);
    shader->set("material.roughness", 2);
    shader->set("material.ambient_occlusion", 3);
    shader->set("material.emissive", 0);

    shader->set("material.albedo_tint", {1,1,1});
    shader->set("material.specular_tint", {1,1,1});
    shader->set("material.roughness_scale", 1.0f);
    shader->set("material.emissive_tint", {0,0,0});
    particleshader->set("offset",{0,0,-2});
    glUniform4f(particleshader->getUniformLocation("color"), 1.0f, 0.0f, 0.0f,0.0f);
            
    glm::mat4 ground_mat = glm::scale(glm::mat4(), glm::vec3(50, 1, 50));
    shader->set("M", ground_mat);
    shader->set("M_it", glm::transpose(glm::inverse(ground_mat)));
     for(int i = 0; i < 2; i++){
        glActiveTexture(GL_TEXTURE0+i);
         planetex[i]->bind();
    }
    ground->draw();   


    glm::mat4 box_mat = glm::translate(glm::mat4(),{Box1Position.x,0,Box1Position.z}); 
    glm::mat4 box_mat_scale = glm::scale(glm::mat4(), glm::vec3(0.09, 0.09, 0.09));
    shader->set("M", box_mat * box_mat_scale);
    shader->set("M_it", glm::transpose(glm::inverse(box_mat)));
    for(int i = 0; i < 5; i++){
        glActiveTexture(GL_TEXTURE0+i);
         boxtex[i]->bind();
    }
    box->draw();

    glm::mat4 box_mat_2= glm::translate(glm::mat4(), {Box2Position.x,0,Box2Position.z}); 
    glm::mat4 box_mat_scale_2 = glm::scale(glm::mat4(), glm::vec3(0.09, 0.09, 0.09));
    shader->set("M", box_mat_2 * box_mat_scale_2);
    shader->set("M_it", glm::transpose(glm::inverse(box_mat_2*box_mat_scale_2)));
    glActiveTexture(GL_TEXTURE0+0);
	boxtex2[0]->bind();
    box2->draw();

    glm::mat4 box_mat_3= glm::translate(glm::mat4(), {Box2Position.x,0,Box2Position.z}); 
    glm::mat4 box_mat_scale_3 = glm::scale(glm::mat4(), glm::vec3(0.09, 0.09, 0.09));
    shader->set("M", box_mat_3 * box_mat_scale_3);
    shader->set("M_it", glm::transpose(glm::inverse(box_mat_2*box_mat_scale_2)));
    glActiveTexture(GL_TEXTURE0+0);
	boxtex2[0]->bind();
    box3->draw();

    glm::mat4 finishline_mat = glm::translate(glm::mat4(), {LinePosition.x,0,LinePosition.z});
    glm::mat4 finishline_mat_scale = glm::scale(glm::mat4(), glm::vec3(0.5,0.1, 0.1));
    shader->set("M", finishline_mat * finishline_mat_scale);
    shader->set("M_it", glm::transpose(glm::inverse(finishline_mat * finishline_mat_scale)));
    glActiveTexture(GL_TEXTURE0);
    checkers[0]->bind();
    plane->draw();
    //shader->set("u_topHemisphereDirection", {0.0f,1.0f,0.0f});
    //shader->set("u_skyColor", skyColor);
    //ground color brown
    //shader->set("u_groundColor",{1,-200,-255});
    glm::mat4 car_mat = glm::translate(glm::mat4(), {CarPosition.x, 0, CarPosition.z}) * glm::rotate( glm::mat4(), carRotation,  glm::vec3(0.2,20,0));
    glm::mat4 car_mat_scale = glm::scale(glm::mat4(), glm::vec3(0.09, 0.09, 0.09) );
    shader->set("M", car_mat * car_mat_scale);
    shader->set("M_it", glm::transpose(glm::inverse(car_mat * car_mat_scale)));
    glActiveTexture(GL_TEXTURE0+0);
	carTex[0]->bind();
    glActiveTexture(GL_TEXTURE0+1);
	carTex[1]->bind();
	car->draw();

    /*glm::mat4 part_mat = glm::scale(glm::mat4(), glm::vec3(0.09, 0.09, 0.09));
    particleshader->set("M", part_mat);
    particleshader->set("M_it", glm::transpose(glm::inverse(part_mat)));         
    glActiveTexture(GL_TEXTURE0);
    parttext[0]->bind();
    part->draw();
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    particleshader->use();
    for (Particle particle : particles)
    {
        if (particle.Life > 0.0f)
        {
            particleshader->set("offset", particle.Position);
            glUniform4f(particleshader->getUniformLocation("color"), particle.Color.a, particle.Color.b, particle.Color.g,1.0f);
            glm::mat4 part_mat = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
            shader->set("M", part_mat);
            shader->set("M_it", glm::transpose(glm::inverse(part_mat)));
            glActiveTexture(GL_TEXTURE0);
            parttext[0]->bind();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

        }
    }
    // Don't forget to reset to default blending mode
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/
    float emissive_power = glm::sin((float)glfwGetTime()) + 1;
    shader->set("material.emissive_tint", glm::vec3(1,1,1) * emissive_power);
  
 ParticleGeneratorDraw();
    //Draw SkyBox
    skyShader->use();
    skyShader->set("VP", VP);
    skyShader->set("cam_pos", cam_pos);
    skyShader->set("M", glm::translate(glm::mat4(), cam_pos));
    skyShader->set("sun_direction", sun_direction);
    skyShader->set("sun_size", 0.02f);
    skyShader->set("sun_halo_size", 0.02f);
    skyShader->set("sun_brightness", 1.0f);
    skyShader->set("sun_color", sunColor);
    skyShader->set("sky_top_color", skyColor);
    skyShader->set("sky_bottom_color", 1.0f-0.25f*(1.0f-skyColor));
    skyShader->set("sky_smoothness", 0.5f);
    glCullFace(GL_FRONT);
    sky->draw();
    glCullFace(GL_BACK);
}

void DirectionalLightScene::Finalize() {
    delete controller;
    delete camera;
    delete sky;
    delete ground;
    for(int i = 0; i < 5; i++){
        delete metal[i];
        delete wood[i];
        delete asphalt[i];
      //  delete checkers[i];
    }
    delete car;
    delete box;
    delete box2;
    delete plane;
    delete skyShader;
    delete particleshader;
    delete shader;
}