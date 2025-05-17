#pragma once

namespace test
{
    class Test
    {
    private:
        float m_Framerate = 0.0f;

    public:
        Test() = default;
        virtual ~Test() = default;

        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender();
        virtual void DisplayFramerate();
        void SetFramerate(float framerate);
    };
}
