#pragma once
#include <vector>
#include <functional>
#include <string>
#include <memory>
#include "Test.h"

namespace test
{

    class TestMenu : public Test
    {
    public:
        TestMenu();
        ~TestMenu();

        void OnImGuiRender() override;

        template <typename T>
        void RegisterTest(const std::string &name)
        {
            m_Tests.push_back(std::make_pair(name, []()
                                             { return std::make_unique<T>(); }));
        }

    public:
        Test *GetCurrentTest() { return m_CurrentTest.get(); }

    private:
        std::vector<std::pair<std::string, std::function<std::unique_ptr<Test>()>>> m_Tests;
        std::unique_ptr<Test> m_CurrentTest;
    };

}
