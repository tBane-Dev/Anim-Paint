#include "History.hpp"
#include "Animation/Frame.hpp"
#include "Animation/Layer.hpp"
#include "DebugLog.hpp"
#include "Components/AnimationsPanel.hpp"
#include "Components/FramesPanel.hpp"
#include "Components/LayersPanel/LayersPanel.hpp"
#include "Components/Canvas.hpp"
#include "Tools/Selection.hpp"
#include "Components/MainMenu/MainMenu.hpp"
#include "Components/Toolbar/Toolbar.hpp"

Step::Step() {
	_animations.clear();
	_canvasResized = false;
}

Step::~Step() {

}

History::History() {
	_currentStep = -1;
	_steps.clear();
}

History::~History() {

}

void History::clear() {
	_currentStep = -1;
	_steps.clear();
}

void History::saveStep() {

	// if we are not at the end of the history, we need to erase the future steps
	if (_currentStep + 1 < (int)(_steps.size())) {
		_steps.erase(_steps.begin() + (_currentStep + 1), _steps.end());
	}
	
	DebugLog(L"History::saveStep: currentStep=" + std::to_wstring(_currentStep) + L", stepsCount=" + std::to_wstring(_steps.size()));	

	// save the current state of the animations
	std::shared_ptr<Step> step = std::make_shared<Step>();

	for (auto& animation : animations) {
		
		std::shared_ptr<Animation> copied_animation = std::make_shared<Animation>();

		for(auto& frame : animation->_frames) {
			
			std::shared_ptr<Frame> copied_frame = std::make_shared<Frame>();
			
			for (auto& layer : frame->_layers) {
				std::shared_ptr<Layer> copied_layer = std::make_shared<Layer>(layer);
				copied_frame->addLayer(copied_layer);
			}
			copied_animation->addFrame(copied_frame);
		}
		step->_animations.push_back(copied_animation);
	}

	step->_currentAnimation = getCurrentAnimationId();
	step->_currentFrame = getCurrentAnimation()->getCurrentFrameID();
	step->_currentLayer = getCurrentAnimation()->getCurrentLayerID();


	step->_edgePoint = canvas->_clickedEdgePoint;
	if (step->_edgePoint != nullptr) {
		step->resize_delta = canvas->_clickedEdgePoint->getPosition() - canvas->_orginalEdgePointPosition;
		std::wcout << step->resize_delta.x << L"," <<  step->resize_delta.y;
	}

	_steps.push_back(step);
	_currentStep++;

}

bool History::canUndo() {
	return (_currentStep > 0 || (resizable_tool != nullptr && resizable_tool->_state != ResizableToolState::None));
}

bool History::canRedo() {
	return _currentStep < (int)(_steps.size())-1;
}

void History::undo()
{

	if (resizable_tool != nullptr && resizable_tool->_state != ResizableToolState::None) {
		DebugLog(L"save resizable tool");
		resizable_tool->pasteToCanvas();
		resizable_tool->reset();
		return;
	}
	else if (!canUndo()) return;

	_currentStep-=1;

	std::shared_ptr<Step> step = _steps[_currentStep];

	animations.clear();

	for (auto& srcAnim : step->_animations) {
		std::shared_ptr<Animation> dstAnim = std::make_shared<Animation>();
		for (auto& srcFrame : srcAnim->_frames) {
			std::shared_ptr<Frame> dstFrame = std::make_shared<Frame>();
			for (auto& srcLayer : srcFrame->_layers) {
				std::shared_ptr<Layer> dstLayer = std::make_shared<Layer>(srcLayer);
				dstFrame->addLayer(dstLayer);
			}
			dstAnim->addFrame(dstFrame);
		}
		animations.push_back(dstAnim);
	}

	currentAnimationId = step->_currentAnimation;
	getCurrentAnimation()->setCurrentFrameID(step->_currentFrame);
	getCurrentAnimation()->setCurrentLayerID(step->_currentLayer);

	animations_panel->updateText();
	frames_panel->updateText();
	layers_panel->loadLayersFromCurrentFrame();

	getCurrentAnimation()->getCurrentLayer()->generateTexture();

	if (_steps[_currentStep + 1]->_edgePoint) {

		step = _steps[_currentStep + 1];

		float scale = canvas->_zoom * canvas->_zoom_delta;

		sf::Vector2i dst;
		dst.x = std::min(step->resize_delta.x, 0) / scale;
		dst.y = std::min(step->resize_delta.y, 0) / scale;

		sf::Vector2i start = canvas->_point_left_top->_rect.position;
		sf::Vector2i end = canvas->_point_right_bottom->_rect.position;

		if (step->_edgePoint.get() == canvas->_point_left_top.get()) {
			start.x -= step->resize_delta.x;
			start.y -= step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_right_top.get()) {
			end.x -= step->resize_delta.x;
			start.y -= step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_left_bottom.get()) {
			start.x -= step->resize_delta.x;
			end.y -= step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_right_bottom.get()) {
			end.x -= step->resize_delta.x;
			end.y -= step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_top.get()) {
			start.y -= step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_bottom.get()) {
			end.y -= step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_left.get()) {
			start.x -= step->resize_delta.x;
		}
		else if (step->_edgePoint.get() == canvas->_point_right.get()) {
			end.x -= step->resize_delta.x;
		}

		canvas->_point_left_top->setPosition(start);
		canvas->_point_top->setPosition({ (start.x + end.x) / 2, start.y });
		canvas->_point_right_top->setPosition({ end.x, start.y });
		canvas->_point_left->setPosition({ start.x, (start.y + end.y) / 2 });
		canvas->_point_right->setPosition({ end.x, (start.y + end.y) / 2 });
		canvas->_point_left_bottom->setPosition({ start.x, end.y });
		canvas->_point_bottom->setPosition({ (start.x + end.x) / 2, end.y });
		canvas->_point_right_bottom->setPosition(end);

		canvas->_rect.position = canvas->_point_left_top->getPosition();
		canvas->_size = sf::Vector2i(sf::Vector2f(end - start) / scale);
		canvas->generateBackground(canvas->_size);
		canvas->setGenerateBackgroundAllCanvases(canvas->_size);
		canvas->setPositionAllCanvases(canvas->_rect.position);
		const size_t framesCount = getCurrentAnimation()->getFrames().size();

		for (size_t f = 0; f < framesCount; ++f) {
			std::shared_ptr<Frame> org = getCurrentAnimation()->getFrames()[f];

			const size_t layersCount = org->getLayers().size();

			for (size_t l = 0; l < layersCount; ++l) {
				std::shared_ptr<Layer> orgLayer = org->getLayers()[l];

				sf::Image newImage;
				sf::Vector2u newSize = sf::Vector2u(canvas->_size);

				newImage.resize(newSize, sf::Color::Transparent);

				// dst jest ujemne przy przesunięciu lewo/góra,
				// więc wklejamy stary obraz z przeciwnym znakiem.
				pasteImageWithAlpha(newImage, orgLayer->_image, 0, 0);

				orgLayer->_image = newImage;
				orgLayer->generateTexture();
			}
		}

		getCurrentAnimation()->getCurrentLayer()->generateTexture();
	}
	

	main_menu->edit_undo->setActive(canUndo());
	main_menu->edit_redo->setActive(canRedo());
}

void History::redo()
{
	if (!canRedo()) return;

	_currentStep+=1;

	std::shared_ptr<Step> step = _steps[_currentStep];

	animations.clear();

	for (auto& srcAnim : step->_animations) {
		std::shared_ptr<Animation> dstAnim = std::make_shared<Animation>();
		for (auto& srcFrame : srcAnim->_frames) {
			std::shared_ptr<Frame> dstFrame = std::make_shared<Frame>();
			for (auto& srcLayer : srcFrame->_layers) {
				std::shared_ptr<Layer> dstLayer = std::make_shared<Layer>(srcLayer);
				dstFrame->addLayer(dstLayer);
			}
			dstAnim->addFrame(dstFrame);
		}
		animations.push_back(dstAnim);
	}

	currentAnimationId = step->_currentAnimation;
	getCurrentAnimation()->setCurrentFrameID(step->_currentFrame);
	getCurrentAnimation()->setCurrentLayerID(step->_currentLayer);

	animations_panel->updateText();
	frames_panel->updateText();
	layers_panel->loadLayersFromCurrentFrame();

	if (_steps[_currentStep]->_edgePoint) {

		step = _steps[_currentStep];

		float scale = canvas->_zoom * canvas->_zoom_delta;

		sf::Vector2i dst;
		dst.x = std::min(step->resize_delta.x, 0) / scale;
		dst.y = std::min(step->resize_delta.y, 0) / scale;

		sf::Vector2i start = canvas->_point_left_top->_rect.position;
		sf::Vector2i end = canvas->_point_right_bottom->_rect.position;

		if (step->_edgePoint.get() == canvas->_point_left_top.get()) {
			start.x += step->resize_delta.x;
			start.y += step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_right_top.get()) {
			end.x += step->resize_delta.x;
			start.y += step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_left_bottom.get()) {
			start.x += step->resize_delta.x;
			end.y += step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_right_bottom.get()) {
			end.x += step->resize_delta.x;
			end.y += step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_top.get()) {
			start.y += step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_bottom.get()) {
			end.y += step->resize_delta.y;
		}
		else if (step->_edgePoint.get() == canvas->_point_left.get()) {
			start.x += step->resize_delta.x;
		}
		else if (step->_edgePoint.get() == canvas->_point_right.get()) {
			end.x += step->resize_delta.x;
		}

		canvas->_point_left_top->setPosition(start);
		canvas->_point_top->setPosition({ (start.x + end.x) / 2, start.y });
		canvas->_point_right_top->setPosition({ end.x, start.y });
		canvas->_point_left->setPosition({ start.x, (start.y + end.y) / 2 });
		canvas->_point_right->setPosition({ end.x, (start.y + end.y) / 2 });
		canvas->_point_left_bottom->setPosition({ start.x, end.y });
		canvas->_point_bottom->setPosition({ (start.x + end.x) / 2, end.y });
		canvas->_point_right_bottom->setPosition(end);

		canvas->_rect.position = canvas->_point_left_top->getPosition();
		canvas->_size = sf::Vector2i(sf::Vector2f(end - start) / scale);
		canvas->generateBackground(canvas->_size);
		canvas->setGenerateBackgroundAllCanvases(canvas->_size);
		canvas->setPositionAllCanvases(canvas->_rect.position);

		const size_t framesCount = getCurrentAnimation()->getFrames().size();

		for (size_t f = 0; f < framesCount; ++f) {
			std::shared_ptr<Frame> org = getCurrentAnimation()->getFrames()[f];

			const size_t layersCount = org->getLayers().size();

			for (size_t l = 0; l < layersCount; ++l) {
				std::shared_ptr<Layer> orgLayer = org->getLayers()[l];

				sf::Image newImage;
				sf::Vector2u newSize = sf::Vector2u(canvas->_size);

				newImage.resize(newSize, sf::Color::Transparent);

				// dst jest ujemne przy przesunięciu lewo/góra,
				// więc wklejamy stary obraz z przeciwnym znakiem.
				pasteImageWithAlpha(newImage, orgLayer->_image, 0, 0);

				orgLayer->_image = newImage;
				orgLayer->generateTexture();
			}
		}

		getCurrentAnimation()->getCurrentLayer()->generateTexture();
	}

	main_menu->edit_redo->setActive(canRedo());
	main_menu->edit_undo->setActive(canUndo());

}

void History::handleEvent(const sf::Event& event) {
	if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp && kp->code == sf::Keyboard::Key::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
		undo();
	}

	if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp && kp->code == sf::Keyboard::Key::Y && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
		redo();
	}
}

std::shared_ptr<History> history;