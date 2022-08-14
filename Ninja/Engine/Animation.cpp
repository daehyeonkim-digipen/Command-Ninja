/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Animation.cpp
Project: PROJECT
Author: Kevin Wright
Creation date: 2/12/2021
-----------------------------------------------------------------*/
#include <fstream>	// file input stuff
#include <filesystem>

#include "Engine.h"	// Engine::GetLogger
#include "Animation.h"

PROJECT::Animation::Animation() : Animation("./assets/none.anm")  { }

PROJECT::Animation::Animation(const std::filesystem::path& fileName) : animSequenceIndex(0) {
	std::ifstream inFile(fileName);
	std::string label;

	if (fileName.extension() != ".anm") {
		throw std::runtime_error("Bad Filetype.  " + fileName.generic_string() + " not a sprite info file (.anm)");
	}
	if (inFile.is_open() == false) {
		throw std::runtime_error("Failed to load " + fileName.generic_string());
	}

	while (inFile.eof() == false) {
		inFile >> label;
		if (label == "PlayFrame") {
			int frame;
			float targetTime;
			inFile >> frame;
			inFile >> targetTime;

			animation.push_back(DEBUG_NEW PlayFrame(frame, targetTime));
		} else if (label == "Loop") {
			int loopToIndex;
			inFile >> loopToIndex;
			animation.push_back(DEBUG_NEW Loop(loopToIndex));
		} else if (label == "End") {
			animation.push_back(DEBUG_NEW End());
		} else {
			Engine::GetLogger().LogError("Unknown command " + label + " in anm file " + fileName.generic_string());
		}
	}
	ResetAnimation();
}

PROJECT::Animation::~Animation() {
	for (CommandData* command : animation) {
		delete command;
	}
	animation.clear();
}

void PROJECT::Animation::Update(double dt) {
	currPlayFrameData->Update(dt);
	if (currPlayFrameData->IsFrameDone() == true) {
		currPlayFrameData->ResetTime();
		++animSequenceIndex;
		if (animSequenceIndex == animation.size()) {
			isAnimationDone = true;
			return;
		}

		if (animation[animSequenceIndex]->GetType() == Command::PlayFrame) {
			currPlayFrameData = static_cast<PlayFrame*>(animation[animSequenceIndex]);
		} else if (animation[animSequenceIndex]->GetType() == Command::Loop) {
			Loop *loopData = static_cast<Loop*>(animation[animSequenceIndex]);
			animSequenceIndex = loopData->GetLoopToIndex();
			if (animation[animSequenceIndex]->GetType() == Command::PlayFrame) {
				currPlayFrameData = static_cast<PlayFrame*>(animation[animSequenceIndex]);
			} else {
				Engine::GetLogger().LogError("Loop does not go to PlayFrame");
				ResetAnimation();
			} 
		} else if (animation[animSequenceIndex]->GetType() == Command::End) {
			isAnimationDone = true;
			return;
		}
	}
}

int PROJECT::Animation::GetDisplayFrame() {
	return currPlayFrameData->GetFrameNum();
}

void PROJECT::Animation::ResetAnimation() {
	animSequenceIndex = 0;
	currPlayFrameData = static_cast<PlayFrame*>(animation[animSequenceIndex]);
	isAnimationDone = false;
}

bool PROJECT::Animation::IsAnimationDone() {
	return isAnimationDone;
}

PROJECT::Animation::PlayFrame::PlayFrame(int frame, double targetTime) : frame(frame), targetTime(targetTime), timer(0) {}
void PROJECT::Animation::PlayFrame::Update(double dt) { timer += dt; }
bool PROJECT::Animation::PlayFrame::IsFrameDone() { return timer >= targetTime; }
void PROJECT::Animation::PlayFrame::ResetTime() { timer = 0; }
int PROJECT::Animation::PlayFrame::GetFrameNum() { return frame; }

PROJECT::Animation::Loop::Loop(int loopToIndex) : loopToIndex(loopToIndex) {}
int PROJECT::Animation::Loop::GetLoopToIndex() {return loopToIndex; }
