#include "Test.h"
#include "imgui.h"

namespace test
{

    void Test::OnImGuiRender()
    {
        DisplayFramerate();
    }

    void Test::DisplayFramerate()
    {
        ImGui::Text("帧率: %.1f FPS", m_Framerate);
    }

    void Test::SetFramerate(float framerate)
    {
        m_Framerate = framerate;
    }

} // namespace test
