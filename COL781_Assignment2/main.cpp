#include "helper.h"

// Callbacks to process input and manage input
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

int main()
{
    // Configuring GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Making the drone camera window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Drone camera", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Making it current context and setting callbacks
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Making fix camera for monitoring drone
    GLFWwindow* window2 = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fixed camera", NULL, NULL);
    if (window2 == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window2, framebuffer_size_callback);

    // Take input from mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    // Vertices to draw lights(cubic)
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // Setting up scene for drone window
    Shader ourShader("shader.vert", "shader.frag");
    Shader lightShader("lightshader.vert", "lightshader.frag");
    Model room("Blender/model5.obj");
    light onlyLight(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(0.4f), glm::vec3(1.0f), glm::vec3(0.3f), 30.0f);

    vector<vector<Model>> drone1(4, vector<Model>());

    drone1[0].push_back(Model("Blender/model1.obj"));

    for (int i = 1; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            drone1[i].push_back(Model("Blender/model" + to_string(i + 1) + ".obj"));
        }
    }

    // Generate light for drone window
    unsigned int lightVAO, VBO;

    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(lightVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Switch to fixed window
    glfwMakeContextCurrent(window2);

    glEnable(GL_DEPTH_TEST);

    // Setting up scene for fixed window
    Shader ourShader2("shader.vert", "shader.frag");
    Shader lightShader2("lightshader.vert", "lightshader.frag");
    Model room2("Blender/model5.obj");
    light onlyLight2(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(0.4f), glm::vec3(1.0f), glm::vec3(0.3f), 30.0f);

    vector<vector<Model>> drone2(4, vector<Model>());

    drone2[0].push_back(Model("Blender/model1.obj"));

    for (int i = 1; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            drone2[i].push_back(Model("Blender/model" + to_string(i + 1) + ".obj"));
        }
    }

    // Generate light for fixed window
    unsigned int lightVAO2, VBO2;

    glGenVertexArrays(1, &lightVAO2);
    glGenBuffers(1, &VBO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(lightVAO2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glfwMakeContextCurrent(window);

    // Rendering the windows
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        glfwMakeContextCurrent(window);

        // Use inputs in drone window only
        processInput(window);

        // clear required buffer bits
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get model transformation and render the drone
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cam.GetViewMatrix();
        glm::mat4 model1 = glm::mat4(1.0f);
        glm::vec3 positn = cam.Position + glm::vec3(0.0f,1.00f,0.0f);
        model1 = glm::translate(model1, positn);
        model1 = glm::rotate(model1, glm::radians(-cam.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model1 = glm::rotate(model1, glm::radians(cam.Pitch), glm::vec3(0.0f, 0.0f, 1.0f));
        model1 = glm::rotate(model1, glm::radians(cam.Roll), glm::vec3(1.0f, 0.0f, 0.0f));
        model1 = glm::scale(model1, glm::vec3(0.5f, 1.0f, 1.25f));
        drawMainBody(ourShader, model1, projection, view, drone1[0][0], onlyLight);

        for (int i = 0; i < 4; i++)
        {
            drawParts(ourShader, model1, projection, view, drone1[1][i], drone1[2][i], drone1[3][i], i + 1, onlyLight);
        }

        // Make room
        glm::mat4 roomModel = glm::mat4(1.0f);
        roomModel = glm::scale(roomModel, glm::vec3(20.0f, 20.0f, 20.0f));
        ourShader.setMat4("model", roomModel);
        room.Draw(ourShader, onlyLight);

        glm::mat4 modelLight = glm::mat4(1.0f);
        modelLight = glm::translate(modelLight, onlyLight.position);
        modelLight = glm::scale(modelLight, glm::vec3(0.2f));

        drawLights(lightVAO, lightShader, modelLight, view, projection);

        // Render the scene for fixed window
        glfwMakeContextCurrent(window2);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader2.use();
        glm::mat4 projection2 = glm::perspective(glm::radians(fixcam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view2 = fixcam.GetViewMatrix();
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::scale(model2, glm::vec3(0.5f, 1.0f, 1.25f));
        drawMainBody(ourShader2, model1, projection2, view2, drone2[0][0], onlyLight2);

        for (int i = 0; i < 4; i++)
        {
            drawParts(ourShader2, model1, projection2, view2, drone2[1][i], drone2[2][i], drone2[3][i], i + 1, onlyLight2);
        }

        ourShader2.setMat4("model", roomModel);
        room2.Draw(ourShader2, onlyLight2);
        drawLights(lightVAO2, lightShader2, modelLight, view2, projection2);

        // Swapping buffers
        glfwSwapBuffers(window);
        glfwSwapBuffers(window2);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

/// <summary>
/// Processing keyboard inputs
/// Escape  - To exit simulation
/// W       - To move drone up
/// S       - To move drone down
/// A       - To roll drone in left (clockwise)
/// D       - To roll drone in right (anti-clockwise)
/// </summary>
/// <param name="window"></param>
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.ProcessKeyboard(RIGHT, deltaTime);
    }
}

/// <summary>
/// Resets window size whenever resized
/// </summary>
/// <param name="window"></param>
/// <param name="width"></param>
/// <param name="height"></param>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/// <summary>
/// Move mouse left and right to manage the YAW
/// Move mouse up and down to manage the PITCH
/// </summary>
/// <param name="window"></param>
/// <param name="xposIn"></param>
/// <param name="yposIn"></param>
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    cam.ProcessMouseMovement(xoffset, yoffset);
}

/// <summary>
/// Process scroll input to manage zoom
/// </summary>
/// <param name="window"></param>
/// <param name="xoffset"></param>
/// <param name="yoffset"></param>
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.ProcessMouseScroll(static_cast<float>(yoffset));
}