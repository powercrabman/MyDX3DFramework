#pragma once

class GameObject;

//-------------------------------------
// 씬은 유일하게 존재해야함
//-------------------------------------

class Scene
{
public:
	virtual ~Scene() = default;

	//씬 메인 함수							
	void EnterSceneCore();
	void UpdateSceneCore(float inDeltaTime);
	void ExitSceneCore();


    template <typename ObjType, typename... Args>
    inline GameObject* RegisterGameObject(Args&&... args)
    {
        static_assert(std::is_base_of<GameObject, ObjType>::value, "RegisterGameObject must use GameObject Based Class.");

        std::unique_ptr<GameObject> gObj = std::make_unique<ObjType>(std::forward<Args>(args...));
        GameObject* gPtr = gObj.get();

        CM::TypeID id = CM::TypeTrait<ObjType>::GetInfo().ID;
        auto repo = m_objRepo.find(id);

        if (repo != m_objRepo.end())
        {
            repo->second.push_back(std::move(gObj));
        }
        else 
        {
            std::vector<std::unique_ptr<GameObject>> vec;
            vec.reserve(32); 
            vec.push_back(std::move(gObj));
            m_objRepo[id] = std::move(vec);
        }

        m_updateObjRepo.push_back(gPtr);
        return gPtr;
    }

protected:
	Scene() = default;

	virtual void EnterScene() = 0;
	virtual void UpdateScene(float inDeltaTime) = 0;
	virtual void ExitScene() = 0;
private:
	CM::TypeInfo m_typeInfo = {};

	//오브젝트를 저장하는 컨테이너
	std::unordered_map<CM::TypeID, std::vector<std::unique_ptr<GameObject>>> m_objRepo;

	//오브젝트를 순회하는 컨테이너
	std::vector<GameObject*> m_updateObjRepo;
};