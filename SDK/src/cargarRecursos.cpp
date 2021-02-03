#include "lgOgre.h"
#include <iostream>
Ogre::String lgOgre::getDefaultMediaDir()
{
    return Ogre::FileSystemLayer::resolveBundlePath(OGRE_MEDIA_DIR);
}
void lgOgre::cargarRecursos()
{
    auto &rgm = Ogre::ResourceGroupManager::getSingleton();
    // load resource paths from config file
    Ogre::ConfigFile cf;
    Ogre::String resourcesPath = sistemaArchivos->getConfigFilePath("resources.cfg");
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    Ogre::Archive *apk = Ogre::ArchiveManager::getSingleton().load("", "APKFileSystem", true);
    cf.load(apk->open(resourcesPath));
#else

    if (Ogre::FileSystemLayer::fileExists(resourcesPath) || OGRE_PLATFORM == OGRE_PLATFORM_EMSCRIPTEN)
    {
        cf.load(resourcesPath);
    }
    else
    {
        rgm.addResourceLocation(getDefaultMediaDir(), "FileSystem", Ogre::RGN_DEFAULT);
    }

#endif

    Ogre::String sec, type, arch;
    // go through all specified resource groups
    Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
    for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci)
    {
        sec = seci->first;
        const Ogre::ConfigFile::SettingsMultiMap &settings = seci->second;
        Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

        // go through all resource paths
        for (i = settings.begin(); i != settings.end(); i++)
        {
            type = i->first;
            arch = i->second;

            Ogre::StringUtil::trim(arch);
            if (arch.empty() || arch[0] == '.')
            {
                // resolve relative path with regards to configfile
                Ogre::String baseDir, filename;
                Ogre::StringUtil::splitFilename(resourcesPath, filename, baseDir);
                arch = baseDir + arch;
            }

            arch = Ogre::FileSystemLayer::resolveBundlePath(arch);

            rgm.addResourceLocation(arch, type, sec);
        }
    }

    if (rgm.getResourceLocationList(Ogre::RGN_INTERNAL).empty())
    {
        const auto &mediaDir = getDefaultMediaDir();
        // add default locations
        rgm.addResourceLocation(mediaDir + "/ShadowVolume", "FileSystem", Ogre::RGN_INTERNAL);
#ifdef OGRE_BUILD_COMPONENT_TERRAIN
        rgm.addResourceLocation(mediaDir + "/Terrain", "FileSystem", Ogre::RGN_INTERNAL);
#endif
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
        rgm.addResourceLocation(mediaDir + "/RTShaderLib/materials", "FileSystem", Ogre::RGN_INTERNAL);
        rgm.addResourceLocation(mediaDir + "/RTShaderLib/GLSL", "FileSystem", Ogre::RGN_INTERNAL);
        rgm.addResourceLocation(mediaDir + "/RTShaderLib/HLSL_Cg", "FileSystem", Ogre::RGN_INTERNAL);
#endif
    }

    sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
    const Ogre::ResourceGroupManager::LocationList genLocs = rgm.getResourceLocationList(sec);

    OgreAssert(!genLocs.empty(), ("Resource Group '" + sec + "' must contain at least one entry").c_str());

    arch = genLocs.front().archive->getName();

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    arch = Ogre::FileSystemLayer::resolveBundlePath("Contents/Resources/Media");
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
    arch = Ogre::FileSystemLayer::resolveBundlePath("Media");
#else
    arch = Ogre::StringUtil::replaceAll(arch, "Media/../../Tests/Media", "");
    arch = Ogre::StringUtil::replaceAll(arch, "media/../../Tests/Media", "");
#endif
    type = genLocs.front().archive->getType();

    bool hasCgPlugin = false;
    const Ogre::Root::PluginInstanceList &lgPlugins = getRoot()->getInstalledPlugins();
    for (size_t i = 0; i < lgPlugins.size(); i++)
    {
        if (lgPlugins[i]->getName() == "Cg Program Manager")
        {
            hasCgPlugin = true;
            break;
        }
    }

    bool use_HLSL_Cg_shared = hasCgPlugin || Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl");

    // unified shaders are written in GLSL dialect
    rgm.addResourceLocation(arch + "/materials/programs/GLSL", type, sec);

    // Add locations for supported shader languages
    if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
    {
        rgm.addResourceLocation(arch + "/materials/programs/GLSLES", type, sec);
    }
    else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
    {
        rgm.addResourceLocation(arch + "/materials/programs/GLSL120", type, sec);

        if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
        {
            rgm.addResourceLocation(arch + "/materials/programs/GLSL150", type, sec);
        }
        if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl400"))
        {
            rgm.addResourceLocation(arch + "/materials/programs/GLSL400", type, sec);
        }
    }
    else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
    {
        rgm.addResourceLocation(arch + "/materials/programs/HLSL", type, sec);
    }

    if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("spirv"))
    {
        rgm.addResourceLocation(arch + "/materials/programs/SPIRV", type, sec);
    }

    if (hasCgPlugin)
        rgm.addResourceLocation(arch + "/materials/programs/Cg", type, sec);
    if (use_HLSL_Cg_shared)
        rgm.addResourceLocation(arch + "/materials/programs/HLSL_Cg", type, sec);
}
void lgOgre::inicializarRecursos()
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
bool lgOgre::inicializarRTShaderSystem()
{
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
    if (Ogre::RTShader::ShaderGenerator::initialize())
    {
        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

        // Create and register the material manager listener if it doesn't exist yet.
        if (!mMaterialMgrListener)
        {
            mMaterialMgrListener = new OgreBites::SGTechniqueResolverListener(mShaderGenerator);
            Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
        }

        return true;
    }
#endif
    return false;
}
void lgOgre::reconfigurar(const Ogre::String &renderer, Ogre::NameValuePairList &options)
{
    mNextRenderer = renderer;
    Ogre::RenderSystem *rs = lgRoot->getRenderSystemByName(renderer);

    // set all given render system options
    for (Ogre::NameValuePairList::iterator it = options.begin(); it != options.end(); it++)
    {
        rs->setConfigOption(it->first, it->second);
    }
    lgRoot->queueEndRendering(); // break from render loop
}