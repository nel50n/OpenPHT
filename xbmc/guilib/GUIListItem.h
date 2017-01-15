/*!
\file GUIListItem.h
\brief
*/

#ifndef GUILIB_GUILISTITEM_H
#define GUILIB_GUILISTITEM_H

#pragma once

/*
 *      Copyright (C) 2005-2012 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "utils/StdString.h"

#include <map>
#include <string>
#include "utils/Variant.h"

//  Forward
class CGUIListItemLayout;
class CArchive;
class CVariant;

/* PLEX */
#include "PlexTypes.h"
/* END PLEX */

/*!
 \ingroup controls
 \brief
 */
class CGUIListItem
{
public:
  typedef std::map<std::string, std::string> ArtMap;

  enum GUIIconOverlay { ICON_OVERLAY_NONE = 0,
                        ICON_OVERLAY_RAR,
                        ICON_OVERLAY_ZIP,
                        ICON_OVERLAY_LOCKED,
                        ICON_OVERLAY_HAS_TRAINER,
                        ICON_OVERLAY_TRAINED,
                        ICON_OVERLAY_UNWATCHED,
                        ICON_OVERLAY_WATCHED,
                        /* PLEX */
                        ICON_OVERLAY_IN_PROGRESS,
                        /* END PLEX */
                        ICON_OVERLAY_HD};

  CGUIListItem(void);
  CGUIListItem(const CGUIListItem& item);
  CGUIListItem(const CStdString& strLabel);
  virtual ~CGUIListItem(void);
  virtual CGUIListItem *Clone() const { return new CGUIListItem(*this); };

  CGUIListItem& operator =(const CGUIListItem& item);

  virtual void SetLabel(const CStdString& strLabel);
  const CStdString& GetLabel() const;

  void SetLabel2(const CStdString& strLabel);
  const CStdString& GetLabel2() const;

  void SetIconImage(const CStdString& strIcon);
  const CStdString& GetIconImage() const;

  void SetOverlayImage(GUIIconOverlay icon, bool bOnOff=false);
  CStdString GetOverlayImage() const;

  /*! \brief Set a particular art type for an item
   \param type type of art to set.
   \param url the url of the art.
   */
  void SetArt(const std::string &type, const std::string &url);

  /*! \brief set artwork for an item
   \param art a type:url map for artwork
   \sa GetArt
   */
  void SetArt(const ArtMap &art);

  /*! \brief append artwork to an item
   \param art a type:url map for artwork
   \param prefix a prefix for the art, if applicable.
   \sa GetArt
   */
  void AppendArt(const ArtMap &art, const std::string &prefix = "");

  /*! \brief set a fallback image for art
   \param from the type to fallback from
   \param to the type to fallback to
   \sa SetArt
   */
  void SetArtFallback(const std::string &from, const std::string &to);

  /*! \brief clear art on an item
   \sa SetArt
   */
  void ClearArt();

  /*! \brief Get a particular art type for an item
   \param type type of art to fetch.
   \return the art URL, if available, else empty.
   */
  std::string GetArt(const std::string &type) const;

  /*! \brief get artwork for an item
   Retrieves artwork in a type:url map
   \return a type:url map for artwork
   \sa SetArt
   */
  const ArtMap &GetArt() const;

  /*! \brief Check whether an item has a particular piece of art
   Equivalent to !GetArt(type).empty()
   \param type type of art to set.
   \return true if the item has that art set, false otherwise.
   */
  bool HasArt(const std::string &type) const;

  void SetSortLabel(const CStdString &label);
  void SetSortLabel(const CStdStringW &label);
  const CStdStringW &GetSortLabel() const;

  void Select(bool bOnOff);
  bool IsSelected() const;

  bool HasIcon() const;
  bool HasOverlay() const;
  virtual bool IsFileItem() const { return false; };

  void SetLayout(CGUIListItemLayout *layout);
  CGUIListItemLayout *GetLayout();

  void SetFocusedLayout(CGUIListItemLayout *layout);
  CGUIListItemLayout *GetFocusedLayout();

  void FreeIcons();
  void FreeMemory(bool immediately = false);
  void SetInvalid();

  bool m_bIsFolder;     ///< is item a folder or a file

#ifdef __PLEX__
  inline void SetProperty(const CStdString &strKey, const CVariant &value)
  {
    std::string _key = strKey;
    std::transform(_key.begin(), _key.end(), _key.begin(), ::tolower);

    PropertyMap::iterator iter = m_mapProperties.find(_key);
    if (iter == m_mapProperties.end())
    {
      m_mapProperties.insert(make_pair(_key, value));
      SetInvalid();
    }
    else if (iter->second != value)
    {
      iter->second = value;
      SetInvalid();
    }
  }
#else
  void SetProperty(const CStdString &strKey, const CVariant &value);
#endif

  void IncrementProperty(const CStdString &strKey, int nVal);
  void IncrementProperty(const CStdString &strKey, double dVal);

  void ClearProperties();

  /*! \brief Append the properties of one CGUIListItem to another.
   Any existing properties in the current item will be overridden if they
   are set in the passed in item.
   \param item the item containing the properties to append.
   */
  void AppendProperties(const CGUIListItem &item);

  void Archive(CArchive& ar);
  void Serialize(CVariant& value);

#ifdef __PLEX__
  inline bool HasProperty(const CStdString &strKey) const
  {
    std::string _key = strKey;
    std::transform(_key.begin(), _key.end(), _key.begin(), ::tolower);

    PropertyMap::const_iterator iter = m_mapProperties.find(_key);
    if (iter == m_mapProperties.end())
      return false;

    return true;
  }
#else
  bool HasProperty(const CStdString &strKey) const;
#endif

  bool       HasProperties() const { return m_mapProperties.size() > 0; };
  void       ClearProperty(const CStdString &strKey);

#ifdef __PLEX__
  inline CVariant GetProperty(const CStdString &strKey) const
  {
    std::string _key = strKey;
    std::transform(_key.begin(), _key.end(), _key.begin(), ::tolower);

    PropertyMap::const_iterator iter = m_mapProperties.find(_key);
    if (iter == m_mapProperties.end())
      return CVariant(CVariant::VariantTypeNull);

    return iter->second;
  }
#else
  CVariant GetProperty(const CStdString &strKey) const;
#endif

  int GetOverlayImageID() const { return m_overlayIcon; }
  const PropertyMap& GetAllProperties() const { return m_mapProperties; }
  /* END PLEX */

protected:
  CStdString m_strLabel2;     // text of column2
  CStdString m_strIcon;      // filename of icon
  GUIIconOverlay m_overlayIcon; // type of overlay icon

  CGUIListItemLayout *m_layout;
  CGUIListItemLayout *m_focusedLayout;
  bool m_bSelected;     // item is selected or not

#ifndef __PLEX__
  struct icompare
  {
    bool operator()(const CStdString &s1, const CStdString &s2) const;
  };

  typedef std::map<CStdString, CVariant, icompare> PropertyMap;
#endif

  PropertyMap m_mapProperties;


private:
  CStdStringW m_sortLabel;    // text for sorting. Need to be UTF16 for proper sorting
  CStdString m_strLabel;      // text of column1

  ArtMap m_art;
  ArtMap m_artFallbacks;
};
#endif

