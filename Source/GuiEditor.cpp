#include <../Source/GuiEditor.h>
#include <../Source/Settings.h>

/**
 *
 * @brief GuiEditor::GuiEditor
 * @param component
 */
GuiEditor::GuiEditor(juce::Component *component) : editableComponent(component), selectedComponent(nullptr)
{
    editableComponent->addMouseListener(this, true);
}

void GuiEditor::paintBackOverlay(Graphics &g)
{
    static constexpr auto cellSize = 10;
    static constexpr auto netThickness = 3;
    static constexpr auto selectionThickness = 4;

    g.setColour(Settings::GUI_EDITOR_COLOR);
    // vertical lines
    for (int i = 0; i < editableComponent->getWidth(); i += cellSize) {
        g.drawLine(i, 0, i, editableComponent->getHeight(), netThickness);
    }

    // horisontal lines
    for (int i = 0; i < editableComponent->getHeight(); i += cellSize) {
        g.drawLine(0, i, editableComponent->getWidth(), i, netThickness);
    }

    if (selectedComponent == nullptr) {
        return;
    }
    // selected component
    g.setColour(Settings::GUI_EDITOR_SELECTION_COLOR);
    g.drawRect(selectedComponent->getBounds(), selectionThickness);
}

void GuiEditor::paintFrontOverlay(Graphics &g)
{
}

void GuiEditor::addToGuiEditor(Component *component)
{
    components.push_back(component);
}

void GuiEditor::initOverlay()
{
    setBounds(0, 0, editableComponent->getWidth(), editableComponent->getHeight());
}

void GuiEditor::mouseDown(const MouseEvent &event)
{
    auto position = event.getPosition();
    auto component = findComponentAt(position);

    selectedComponent = component;
}

void GuiEditor::mouseUp(const MouseEvent &event)
{
    selectedComponent = nullptr;
}

void GuiEditor::mouseMove(const MouseEvent &event)
{
    // hover on element
}

void GuiEditor::mouseDrag(const MouseEvent &event)
{
    auto position = event.getPosition();
    if (selectedComponent == nullptr) {
        return;
    }

    // set bounds of center of component, not of upper left corner
    auto newXOffset = selectedComponent->getWidth() / 2;
    auto newYOffset = selectedComponent->getHeight() / 2;
    selectedComponent->setBounds(position.x - newXOffset, position.y - newYOffset,
                                 selectedComponent->getWidth(), selectedComponent->getHeight());

    editableComponent->repaint();
}

Component *GuiEditor::findComponentAt(Point<int> position)
{
    for (auto component : components) {
        auto relativePosition = Point<int>(position.getX() - component->getX(), position.getY() - component->getY());
        if (component->contains(relativePosition)) {
            return component;
        }
    }
}
