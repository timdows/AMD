#pragma once
class SceneStatus {
public:
	bool scene1 = false;
	bool scene2 = false;
	bool scene3 = false;
	bool scene4 = false;

	void setAllOff()
	{
		this->scene1 = false;
		this->scene2 = false;
		this->scene3 = false;
		this->scene4 = false;
	}

	void setScene(int scene)
	{
		this->setAllOff();

		switch (scene)
		{
		case 1:
			this->scene1 = true;
			break;
		case 2:
			this->scene2 = true;
			break;
		case 3:
			this->scene3 = true;
			break;
		case 4:
			this->scene4 = true;
			break;
		}
	}

	bool hasRunningScene()
	{
		return this->scene1 || this->scene2 || this->scene3 || this->scene4;
	}
};