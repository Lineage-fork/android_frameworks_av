/*
 * Copyright (C) 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <utils/Errors.h>

#include <aidl/android/hardware/audio/effect/BpEffect.h>
#include <system/audio_effect.h>
#include <system/audio_effects/audio_effects_utils.h>

namespace android {
namespace effect {

class EffectConversionHelperAidl {
  public:
    status_t handleCommand(uint32_t cmdCode, uint32_t cmdSize, void* pCmdData, uint32_t* replySize,
                           void* pReplyData);
    virtual ~EffectConversionHelperAidl() {}

  protected:
    const int32_t mSessionId;
    const int32_t mIoId;
    const ::aidl::android::hardware::audio::effect::Descriptor mDesc;
    const std::shared_ptr<::aidl::android::hardware::audio::effect::IEffect> mEffect;
    ::aidl::android::hardware::audio::effect::IEffect::OpenEffectReturn mOpenReturn;
    ::aidl::android::hardware::audio::effect::Parameter::Common mCommon;

    EffectConversionHelperAidl(
            std::shared_ptr<::aidl::android::hardware::audio::effect::IEffect> effect,
            int32_t sessionId, int32_t ioId,
            const ::aidl::android::hardware::audio::effect::Descriptor& desc);

    status_t handleSetParameter(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                                void* pReplyData);
    status_t handleGetParameter(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                                void* pReplyData);

  private:
    const aidl::android::media::audio::common::AudioFormatDescription kDefaultFormatDescription = {
            .type = aidl::android::media::audio::common::AudioFormatType::PCM,
            .pcm = aidl::android::media::audio::common::PcmType::FLOAT_32_BIT};

    static constexpr int kDefaultframeCount = 0x100;

    using AudioChannelLayout = aidl::android::media::audio::common::AudioChannelLayout;
    const aidl::android::media::audio::common::AudioConfig kDefaultAudioConfig = {
            .base = {.sampleRate = 44100,
                     .channelMask = AudioChannelLayout::make<AudioChannelLayout::layoutMask>(
                             AudioChannelLayout::LAYOUT_STEREO),
                     .format = kDefaultFormatDescription},
            .frameCount = kDefaultframeCount};
    // command handler map
    typedef status_t (EffectConversionHelperAidl::*CommandHandler)(uint32_t /* cmdSize */,
                                                                   const void* /* pCmdData */,
                                                                   uint32_t* /* replySize */,
                                                                   void* /* pReplyData */);
    static const std::map<uint32_t /* effect_command_e */, CommandHandler> mCommandHandlerMap;

    status_t handleInit(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                        void* pReplyData);
    status_t handleSetConfig(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                             void* pReplyData);
    status_t handleGetConfig(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                             void* pReplyData);
    status_t handleEnable(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                          void* pReplyData);
    status_t handleDisable(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                           void* pReplyData);
    status_t handleReset(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                         void* pReplyData);
    status_t handleSetDevice(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                             void* pReplyData);
    status_t handleSetVolume(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                             void* pReplyData);
    status_t handleSetOffload(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                              void* pReplyData);
    status_t handleFirstPriority(uint32_t cmdSize, const void* pCmdData, uint32_t* replySize,
                                 void* pReplyData);

    // implemented by conversion of each effect
    virtual status_t setParameter(utils::EffectParamReader& param) = 0;
    virtual status_t getParameter(utils::EffectParamWriter& param) = 0;
};

}  // namespace effect
}  // namespace android
