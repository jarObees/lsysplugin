#pragma once

// Args: (int width, int height, juce::Image filmstrip, std::string id, bool isSlider=false)
// TODO: Refactor for method chaining and operator overloading.
namespace jive_gui
{
	class FilmstripKnobView : public jive::View
	{
	public:
		FilmstripKnobView(int rawWidth, int rawheight, juce::Image rawTing2, juce::String rawId, bool rawIsSlider = false)
			: width(rawWidth), height(rawheight), rawFilmstripImage(rawTing2), sliderId(rawId), isSlider(rawIsSlider)
		{
			sideLength = rawFilmstripImage.getWidth();
			numFrames = rawFilmstripImage.getHeight() / sideLength;
		}

		juce::ValueTree initialise() final
		{
			juce::String componentType = isSlider ? "Slider" : "Knob";
			return juce::ValueTree{
				componentType,
				{
					{"id", sliderId},
					{"width", width},
					{"height", height},
					{"justify-content", "centre"},
				},
				{
					juce::ValueTree{
						"Image",
						{
							{"source", jive::toVar(rawFilmstripImage)},
						}
					},
				},
			};
		}

		void setup(jive::GuiItem& item) final
		{
			// We confirm access to the slider itself.
			if (auto* stripSlider = dynamic_cast<juce::Slider*>(item.getComponent().get()))
			{
				// Defucks the image component.
				auto* imageComponent = item.getChildren().getFirst()->getComponent().get();
				imageComponent->setInterceptsMouseClicks(false, false);
				onValueChange = std::make_unique<jive::Event>(item.state, "on-change");
				filmstripSource = std::make_unique<jive::Property<juce::Image>>(item.state.getChild(0), "source"); // We can interact with the "source" property of "Image" through this.
				// When it detects a value change, it displays the appropriate cropped portion of the filmstrip image.
				onValueChange->onTrigger = [this, stripSlider]() {
					std::size_t imageNumber = static_cast<std::size_t>
						(0.5 + (stripSlider->getValue() 
								- stripSlider->getMinimum()) 
								/ (stripSlider->getMaximum() 
								- stripSlider->getMinimum()) 
								* (numFrames - 1));
					juce::Rectangle<int> clippedArea(0, imageNumber * sideLength, sideLength, sideLength);
					juce::Image croppedImage = rawFilmstripImage.getClippedImage(clippedArea);
					filmstripSource->set(croppedImage);
					DBG(sliderId << "filmstrip value: " << stripSlider->getValue());
					};
			}
			else
			{
				jassertfalse;
			}
		}

	private:
		bool isSlider;
		int width, height;
		juce::String sliderId;
		size_t numFrames, sideLength;
		juce::Image rawFilmstripImage;
		std::unique_ptr<jive::Event> onValueChange;
		std::unique_ptr<jive::Property<juce::Image>> filmstripSource;
	};
}