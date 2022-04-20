#include "Renderer/Renderer.h"

Renderer::Renderer(Window &window)
    : window_(window), delta_time_(0.0), last_frame_(0.0), first_mouse_(true),
      last_x_((float)window.GetWidth() / 2.0f), last_y_((float)window.GetHeight() / 2.0f)
{
    setupInput(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    setupGlobalEnables();
}

void Renderer::Render(Camera &camera, Player &player, GameWorld &world)
{
    UniformBuffer<glm::mat4> ubo_matrices(3, 0);
    UniformBuffer<glm::vec3> ubo_camera(1, 1);
    UniformBuffer<glm::vec3> ubo_light(1, 2);

    ImGui::StyleColorsDark();
    ImGuiWindowFlags imgui_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar |
                                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;

    ProcessMouse(camera, player, window_.GetWindow(), last_x_, last_y_);
    player.UpdateBoundingBox();
    player.SetTimeLimit(300.0);

    while (!window_.GetWindowShouldClose())
    {
        clearFramebuffers();
        processFrametime();
        processKeyboard(camera, player, world);
        world.RemoveCollectibles(world.quad_tree_.Query(player.GetBoundingBox()), player);
        player.UpdateTimeRemaining(delta_time_);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view_3 = camera.GetViewMatrix3();
        glm::mat4 projection = camera.GetProjectionMatrix();

        ubo_matrices.Data(projection, 0);
        ubo_matrices.Data(view, 1);
        ubo_matrices.Data(view_3, 2);
        ubo_camera.Data(camera.position_, 0);
        ubo_light.Data(world.GetSunPosition(), 0);

        ImGui::Begin("Score", 0, imgui_flags);
        ImGui::Text("%s", player.GetScorePretty().c_str());
        ImGui::Text("%s", player.GetTimeRemainingPretty().c_str());
        ImGui::SetWindowPos(ImVec2(0.f, 0.f));
        ImGui::SetWindowSize(ImVec2(200.f, 75.f));
        ImGui::End();

        ImGui::Begin("Stats", 0, imgui_flags);
        ImGui::SetWindowFontScale(0.5f);
        ImGui::Text("%s", getFps().c_str());
        ImGui::Text("%s", getFrametime().c_str());
        ImGui::SetWindowPos(ImVec2(window_.GetWidth() - 200.f, window_.GetHeight() - 75.f));
        ImGui::SetWindowSize(ImVec2(200.f, 75.f));
        ImGui::End();
        ImGui::Render();

        world.Draw();
        player.Draw();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_.GetWindow());
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void Renderer::ProcessFramebuffer(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    window_.SetWidth(width);
    window_.SetHeight(height);
}

void Renderer::ProcessMouse(Camera &camera,
                            Player &player,
                            GLFWwindow *window,
                            double x_pos,
                            double y_pos)
{
    if (first_mouse_)
    {
        last_x_ = (float)x_pos;
        last_y_ = (float)y_pos;
        first_mouse_ = false;
    }

    float x_offset = (float)x_pos - last_x_;
    float y_offset = last_y_ - (float)y_pos;
    last_x_ = (float)x_pos;
    last_y_ = (float)y_pos;

    player.HandleMouse(camera, x_offset, y_offset);
    camera.SetPlayerPosition(player.position_);
    camera.FollowPlayer();
    camera.HandleMouse(x_offset, y_offset);
}

void Renderer::processKeyboard(Camera &camera, Player &player, GameWorld &world)
{
    if (glfwGetKey(window_.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        window_.SetWindowShouldClose(true);
    }

    float velocity = player.movement_speed_ * (float)delta_time_;
    if (glfwGetKey(window_.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
    {
        player.position_ += player.front_ * velocity;
        player.position_.y = world.GetGridHeight(player.position_);
        player.UpdateBoundingBox();
        camera.SetPlayerPosition(player.position_);
        camera.FollowPlayer();
    }
    if (glfwGetKey(window_.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
    {
        player.position_ -= player.front_ * velocity;
        player.position_.y = world.GetGridHeight(player.position_);
        player.UpdateBoundingBox();
        camera.SetPlayerPosition(player.position_);
        camera.FollowPlayer();
    }
    if (glfwGetKey(window_.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
    {
        player.position_ -= player.right_ * velocity;
        player.position_.y = world.GetGridHeight(player.position_);
        player.UpdateBoundingBox();
        camera.SetPlayerPosition(player.position_);
        camera.FollowPlayer();
    }
    if (glfwGetKey(window_.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
    {
        player.position_ += player.right_ * velocity;
        player.position_.y = world.GetGridHeight(player.position_);
        player.UpdateBoundingBox();
        camera.SetPlayerPosition(player.position_);
        camera.FollowPlayer();
    }
}

void Renderer::setupInput(int mode, int value)
{
    glfwSetInputMode(window_.GetWindow(), mode, value);
}

void Renderer::setupGlobalEnables()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_FRAMEBUFFER_SRGB);

    if (window_.GetMultisamplingEnabled())
    {
        glEnable(GL_MULTISAMPLE);
    }
}

void Renderer::processFrametime()
{
    double now = glfwGetTime();
    delta_time_ = now - last_frame_;
    last_frame_ = now;
}

void Renderer::clearFramebuffers() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

std::string Renderer::getFps() { return "FPS:" + std::to_string(ImGui::GetIO().Framerate); }

std::string Renderer::getFrametime()
{
    return "FT:" + std::to_string(ImGui::GetIO().DeltaTime * 1000.0);
}
