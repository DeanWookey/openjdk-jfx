/*
 * Copyright (C) 2007 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2018 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#pragma once

#include "SVGNames.h"
#include "SVGTextContentElement.h"
#include "SVGURIReference.h"

namespace WebCore {

enum SVGTextPathMethodType {
    SVGTextPathMethodUnknown = 0,
    SVGTextPathMethodAlign,
    SVGTextPathMethodStretch
};

enum SVGTextPathSpacingType {
    SVGTextPathSpacingUnknown = 0,
    SVGTextPathSpacingAuto,
    SVGTextPathSpacingExact
};

template<>
struct SVGPropertyTraits<SVGTextPathMethodType> {
    static unsigned highestEnumValue() { return SVGTextPathMethodStretch; }

    static String toString(SVGTextPathMethodType type)
    {
        switch (type) {
        case SVGTextPathMethodUnknown:
            return emptyString();
        case SVGTextPathMethodAlign:
            return "align"_s;
        case SVGTextPathMethodStretch:
            return "stretch"_s;
        }

        ASSERT_NOT_REACHED();
        return emptyString();
    }

    static SVGTextPathMethodType fromString(const String& value)
    {
        if (value == "align")
            return SVGTextPathMethodAlign;
        if (value == "stretch")
            return SVGTextPathMethodStretch;
        return SVGTextPathMethodUnknown;
    }
};

template<>
struct SVGPropertyTraits<SVGTextPathSpacingType> {
    static unsigned highestEnumValue() { return SVGTextPathSpacingExact; }

    static String toString(SVGTextPathSpacingType type)
    {
        switch (type) {
        case SVGTextPathSpacingUnknown:
            return emptyString();
        case SVGTextPathSpacingAuto:
            return "auto"_s;
        case SVGTextPathSpacingExact:
            return "exact"_s;
        }

        ASSERT_NOT_REACHED();
        return emptyString();
    }

    static SVGTextPathSpacingType fromString(const String& value)
    {
        if (value == "auto")
            return SVGTextPathSpacingAuto;
        if (value == "exact")
            return SVGTextPathSpacingExact;
        return SVGTextPathSpacingUnknown;
    }
};

class SVGTextPathElement final : public SVGTextContentElement, public SVGURIReference {
    WTF_MAKE_ISO_ALLOCATED(SVGTextPathElement);
public:
    // Forward declare enumerations in the W3C naming scheme, for IDL generation.
    enum {
        TEXTPATH_METHODTYPE_UNKNOWN = SVGTextPathMethodUnknown,
        TEXTPATH_METHODTYPE_ALIGN = SVGTextPathMethodAlign,
        TEXTPATH_METHODTYPE_STRETCH = SVGTextPathMethodStretch,
        TEXTPATH_SPACINGTYPE_UNKNOWN = SVGTextPathSpacingUnknown,
        TEXTPATH_SPACINGTYPE_AUTO = SVGTextPathSpacingAuto,
        TEXTPATH_SPACINGTYPE_EXACT = SVGTextPathSpacingExact
    };

    static Ref<SVGTextPathElement> create(const QualifiedName&, Document&);

    const SVGLengthValue& startOffset() const { return m_startOffset.currentValue(attributeOwnerProxy()); }
    SVGTextPathMethodType method() const { return m_method.currentValue(attributeOwnerProxy()); }
    SVGTextPathSpacingType spacing() const { return m_spacing.currentValue(attributeOwnerProxy()); }

    RefPtr<SVGAnimatedLength> startOffsetAnimated() { return m_startOffset.animatedProperty(attributeOwnerProxy()); }
    RefPtr<SVGAnimatedEnumeration> methodAnimated() { return m_method.animatedProperty(attributeOwnerProxy()); }
    RefPtr<SVGAnimatedEnumeration> spacingAnimated() { return m_spacing.animatedProperty(attributeOwnerProxy()); }

protected:
    void didFinishInsertingNode() override;

private:
    SVGTextPathElement(const QualifiedName&, Document&);
    virtual ~SVGTextPathElement();

    using AttributeOwnerProxy = SVGAttributeOwnerProxyImpl<SVGTextPathElement, SVGTextContentElement, SVGURIReference>;
    static AttributeOwnerProxy::AttributeRegistry& attributeRegistry() { return AttributeOwnerProxy::attributeRegistry(); }
    static bool isKnownAttribute(const QualifiedName& attributeName) { return AttributeOwnerProxy::isKnownAttribute(attributeName); }
    static void registerAttributes();

    const SVGAttributeOwnerProxy& attributeOwnerProxy() const final { return m_attributeOwnerProxy; }
    void parseAttribute(const QualifiedName&, const AtomicString&) override;
    void svgAttributeChanged(const QualifiedName&) override;

    RenderPtr<RenderElement> createElementRenderer(RenderStyle&&, const RenderTreePosition&) override;
    bool childShouldCreateRenderer(const Node&) const override;
    bool rendererIsNeeded(const RenderStyle&) override;

    void clearResourceReferences();
    void buildPendingResource() override;
    InsertedIntoAncestorResult insertedIntoAncestor(InsertionType, ContainerNode&) override;
    void removedFromAncestor(RemovalType, ContainerNode&) override;

    bool selfHasRelativeLengths() const override;

    AttributeOwnerProxy m_attributeOwnerProxy { *this };
    SVGAnimatedLengthAttribute m_startOffset { LengthModeOther };
    SVGAnimatedEnumerationAttribute<SVGTextPathMethodType> m_method { SVGTextPathMethodAlign };
    SVGAnimatedEnumerationAttribute<SVGTextPathSpacingType> m_spacing { SVGTextPathSpacingExact };
};

} // namespace WebCore
