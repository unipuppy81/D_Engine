#include "Core/Input.h"

namespace Unipuppy
{
    Input::Input()
    {
        // 배열을 모두 false / 0 으로 초기화합니다.
        ZeroMemory(m_keys, sizeof(m_keys));
        ZeroMemory(m_mouseButtons, sizeof(m_mouseButtons));
        m_mousePosition = POINT{ 0, 0 };
        m_prevMousePosition = POINT{ 0, 0 };
        m_mouseDelta = POINT{ 0, 0 };
    }

    void Input::NewFrame()
    {
        // 마우스 델타는 프레임마다 초기화합니다.
        m_mouseDelta.x = 0;
        m_mouseDelta.y = 0;
    }

    void Input::OnKeyDown(std::uint32_t virtualKey)
    {
        if (virtualKey < 256) m_keys[virtualKey] = true;
    }

    void Input::OnKeyUp(std::uint32_t virtualKey)
    {
        if (virtualKey < 256) m_keys[virtualKey] = false;
    }

    bool Input::IsKeyDown(std::uint32_t virtualKey) const
    {
        if (virtualKey < 256) return m_keys[virtualKey];
        return false;
    }

    void Input::OnMouseMove(int x, int y)
    {
        POINT current{ x, y };

        if (!m_hasPreviousPos)
        {
            m_prevMousePosition = current;
            m_hasPreviousPos = true;
        }

        // 이번 프레임의 델타를 누적합니다.
        m_mouseDelta.x += current.x - m_prevMousePosition.x;
        m_mouseDelta.y += current.y - m_prevMousePosition.y;

        m_prevMousePosition = current;
        m_mousePosition = current;
    }

    void Input::OnMouseDown(int buttonIndex)
    {
        if (buttonIndex >= 0 && buttonIndex < 3)
            m_mouseButtons[buttonIndex] = true;
    }

    void Input::OnMouseUp(int buttonIndex)
    {
        if (buttonIndex >= 0 && buttonIndex < 3)
            m_mouseButtons[buttonIndex] = false;
    }

    bool Input::IsMouseDown(int buttonIndex) const
    {
        if (buttonIndex >= 0 && buttonIndex < 3)
            return m_mouseButtons[buttonIndex];
        return false;
    }
}