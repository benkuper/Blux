#include "Main.h"
#include "UI/AssetManager.h"
#include "Engine/BluxEngine.h"

BluxApplication::BluxApplication() : 
    OrganicApplication(ProjectInfo::projectName, 
        true, 
        BluxAssetManager::getImage("icon3"))
{
    
}

void BluxApplication::initialiseInternal(const String&)
{
    engine.reset(new BluxEngine());
}
