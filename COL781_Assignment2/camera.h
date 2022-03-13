#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Possible movements of camera using keys
enum Camera_Movement {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = 0.0f;
const float PITCH = 0.0f;
const float ROLL = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// Camera object
class camera
{
public:
    
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Used for rotation speed of drone
    vector<float> rotation;
    vector<int> sign;
    float lastXoffset;

    // euler Angles
    float Yaw;
    float Pitch;
    float Roll;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // If camera is not attached then will not move
    bool attached;

    // constructor
    camera(bool isAttached = false, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float roll = ROLL) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        attached = isAttached;
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        Roll = roll;
        rotation = { 1250.0f, 1250.0f, -1250.0f, -1250.0f };
        sign = { 1,1,-1,-1 };
        updateCameraVectors();
    }

    // returns the view matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    /// <summary>
    /// Process keyboard input
    /// </summary>
    /// <param name="direction">Direction in which drone will move or roll</param>
    /// <param name="deltaTime">For smooth movement</param>
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        if (!attached)
            return;

        float velocity = MovementSpeed * deltaTime;
        
        // Move drone up
        if (direction == UP) {
            Position += Up * velocity;
            for (int i = 0; i < 4; i++)
            {
                rotation[i] += 4.0f * sign[i];
            }
        }

        // Move drone down
        if (direction == DOWN) {
            Position -= Up * velocity;
            for (int i = 0; i < 4; i++)
            {
                rotation[i] -= 4.0f * sign[i];
            }
        }

        // Roll drone to the left
        if (direction == LEFT) {
            Roll -= 0.5f;
            rotation[0] += 4.0f * sign[0];
            rotation[2] += 4.0f * sign[2];
            rotation[1] -= 4.0f * sign[1];
            rotation[3] -= 4.0f * sign[3];
        }

        // Roll drone to the right
        if (direction == RIGHT) {
            Roll += 0.5f;
            rotation[0] -= 4.0f * sign[0];
            rotation[2] -= 4.0f * sign[2];
            rotation[1] += 4.0f * sign[1];
            rotation[3] += 4.0f * sign[3];
        }

        // Clamp roll values
        if (Roll > 89.0f)
            Roll = 89.0f;
        if (Roll < -89.0f)
            Roll = -89.0f;

        // Update camera's local co-ordinate system
        updateCameraVectors();
        clampRotation();
    }

    // Control camera's pitch and yaw
    // Move mouse in left and right to control yaw
    // Move mouse in up and down to control pitch
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        if (!attached)
            return;

        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        // Manage fan movements when rotating
        if (xoffset != 0) {
            if (xoffset > 0) {
                rotation[0] -= 4.0f * sign[0];
                rotation[2] += 4.0f * sign[2];
                rotation[1] -= 4.0f * sign[1];
                rotation[3] += 4.0f * sign[3];
            }
            else {
                rotation[0] += 4.0f * sign[0];
                rotation[2] -= 4.0f * sign[2];
                rotation[1] += 4.0f * sign[1];
                rotation[3] -= 4.0f * sign[3];
            }
        }

        Pitch += yoffset;

        // Manage fan movements when rotating
        if (yoffset != 0) {
            if (yoffset > 0) {
                rotation[0] += 4.0f * sign[0];
                rotation[2] -= 4.0f * sign[2];
                rotation[1] -= 4.0f * sign[1];
                rotation[3] += 4.0f * sign[3];
            }
            else {
                rotation[0] -= 4.0f * sign[0];
                rotation[2] += 4.0f * sign[2];
                rotation[1] += 4.0f * sign[1];
                rotation[3] -= 4.0f * sign[3];
            }
        }

        // make sure that drone will not be upside down
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update camera's local co-ordinate system
        updateCameraVectors();
        clampRotation();
    }

    // Manage zoom of camera view
    void ProcessMouseScroll(float yoffset)
    {
        if (!attached)
            return;

        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:

    // Update camera's local co-ordinate system after processing input
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));

        glm::mat4 roll_mat = glm::rotate(glm::mat4(1.0f), glm::radians(Roll), Front);
        Up = glm::mat3(roll_mat) * Up;
        Up = glm::normalize(Up);
    }

    // Clamp rotation so that fans will not move on too high or low speeds
    void clampRotation() {
        for (int i = 0; i < 4; i++)
        {
            if (rotation[i] * sign[i] > 5000.0f) {
                rotation[i] = 5000.0f * sign[i];
            }
            if (rotation[i] * sign[i] < 30.0f) {
                rotation[i] = 30.0f * sign[i];
            }
        }
    }
};
#endif