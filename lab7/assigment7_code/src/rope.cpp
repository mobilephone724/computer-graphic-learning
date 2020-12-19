#include <iostream>
#include <vector>
#include <unistd.h>
#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL
{

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

        // // Comment - in this part when you implement the constructor

        for (int i = 0; i < num_nodes; ++i)
        {
            Mass *new_mass = new Mass(start + (end - start) * (double(i) / num_nodes), node_mass, false);
            masses.push_back(new_mass);
            if (i != 0)
            {
                Spring *new_spring = new Spring(masses[i - 1], masses[i], 100);
                springs.push_back(new_spring);
            }
        }
        for (auto &i : pinned_nodes)
        {
            masses[i]->pinned = true;
        }
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        // for (auto &s : springs)
        // {
        //     // TODO (Part 2): Use Hooke's law to calculate the force on a node
        //     auto d = s->m2->position - s->m1->position;
        //     s->m1->forces += s->k * d / d.norm() * (d.norm() - s->rest_length);
        //     s->m2->forces -= s->k * d / d.norm() * (d.norm() - s->rest_length);
        // }

        // for (auto &m : masses)
        // {
        //     if (!m->pinned)
        //     {
        //         // TODO (Part 2): Add gravity and global damping, then compute the new velocity and position
        //         double kd = 0.1;
        //         Vector2D a = (gravity + m->forces - kd * m->velocity) / m->mass;
        //         auto old_velocity = m->velocity;
        //         m->velocity += a * delta_t;
        //         m->position += m->velocity * delta_t;
        //         // m->position += old_velocity * delta_t;
        //         // std::cout << m->velocity << std::endl;
        //     }

        //     // Reset all forces on each mass
        //     m->forces = Vector2D(0, 0);
        // }
        // sleep(1);
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            // 在此进行质点的位置调整，维持弹簧的原始长度
            auto d = s->m2->position - s->m1->position;
            if (s->m1->pinned != 1)
            {
                s->m1->position += (d.norm() - s->rest_length) / 2 * d / d.norm();
                s->m2->position += -(d.norm() - s->rest_length) / 2 * d / d.norm();
            }
            else
            {
                s->m2->position += -(d.norm() - s->rest_length) * d / d.norm();
            }
        }
        double damp_factor = 0.005;
        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 3): Set the new position of the rope mass
                // 在此计算重力影响下，质点的位置变化
                Vector2D temp_position = m->position;
                m->position += (1 - damp_factor) * (m->position - m->last_position) + gravity * delta_t / 2 * delta_t / 2;
                m->last_position = temp_position;
            }
        }
    }
} // namespace CGL
