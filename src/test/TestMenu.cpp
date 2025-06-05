#include "TestMenu.h"
#include "imgui.h"

namespace test
{

    TestMenu::TestMenu()
        : m_CurrentTest(nullptr)
    {
    }

    TestMenu::~TestMenu()
    {
    }

    void TestMenu::OnImGuiRender()
    {
        if (m_CurrentTest)
        {
            if (ImGui::Button("<-"))
            {
                m_CurrentTest = nullptr;
            }
            else
            {
                m_CurrentTest->OnImGuiRender();
            }
        }
        else
        {
            for (auto &test : m_Tests)
            {
                if (ImGui::Button(test.first.c_str()))
                {
                    m_CurrentTest = test.second();
                }
            }
        }
    }

}
