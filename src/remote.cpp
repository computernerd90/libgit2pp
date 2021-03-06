/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * libgit2pp
 * Copyright (C) 2013-2014 Émilien Kia <emilien.kia@gmail.com>
 *
 * libgit2pp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libgit2pp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

#include "remote.hpp"

#include "common.hpp"
#include "exception.hpp"

namespace git2
{

//
// RefSpec
//

RefSpec::RefSpec(const git_refspec *refspec):
_refspec(refspec)
{
}

RefSpec::~RefSpec()
{
}

std::string RefSpec::src()const
{
	return std::string(git_refspec_src(constData()));
}

std::string RefSpec::dst()const
{
	return std::string(git_refspec_dst(constData()));
}

std::string RefSpec::str()const
{
	return std::string(git_refspec_string(constData()));
}

bool RefSpec::force()const
{
	return git_refspec_force(constData()) != 0;
}

git_direction RefSpec::direction()const
{
	return git_refspec_direction(constData());
}

bool RefSpec::sourceMatches(const std::string& refname)const
{
	return git_refspec_src_matches(constData(), refname.c_str());
}

bool RefSpec::destinationMatches(const std::string& refname)const
{
	return git_refspec_dst_matches(constData(), refname.c_str());
}

std::string RefSpec::transform(const std::string& name)const
{
	git_buf* buffer;
	Exception::git2_assert(git_refspec_transform(buffer, constData(), name.c_str()));
	std::string str(buffer->ptr, buffer->size);
	git_buf_free(buffer);
	return str;
}

std::string RefSpec::rtransform(const std::string& name)const
{
	git_buf* buffer;
	Exception::git2_assert(git_refspec_rtransform(buffer, constData(), name.c_str()));
	std::string str(buffer->ptr, buffer->size);
	git_buf_free(buffer);
	return str;
}

const git_refspec *RefSpec::constData()const
{
	return _refspec;
}

//
// Remote
//

Remote::Remote(git_remote *remote):
_remote(remote)
{
}

Remote::~Remote()
{
	if(_remote!=NULL)
	   git_remote_free(_remote);
}

// void Remote::save()
// {
// 	Exception::git2_assert(git_remote_save(data()));
// }

std::string Remote::name()const
{
	return std::string(git_remote_name(data()));
}

std::string Remote::url()const
{
	return std::string(git_remote_url(data()));
}

std::string Remote::pushUrl()const
{
	return std::string(git_remote_pushurl(data()));
}

std::vector<std::string> Remote::getFetchRefspec()
{
	std::vector<std::string> list;
	git_strarray arr;
	Exception::git2_assert(git_remote_get_fetch_refspecs(&arr, data()));
	helper::push_back(list, &arr);
	git_strarray_free(&arr);
	return list;
}

std::vector<std::string> Remote::getPushRefspec()
{
	std::vector<std::string> list;
	git_strarray arr;
	Exception::git2_assert(git_remote_get_push_refspecs(&arr, data()));
	helper::push_back(list, &arr);
	git_strarray_free(&arr);
	return list;
}

// void Remote::clearRefspec()
// {
// 	git_remote_clear_refspecs(data());
// }

size_t Remote::refspecCount()const
{
	return git_remote_refspec_count(data());
}

RefSpec Remote::getRefspec(size_t n)
{
	return RefSpec(git_remote_get_refspec(data(), n));
}

// void Remote::removeRefspec(size_t n)
// {
// 	Exception::git2_assert(git_remote_remove_refspec(data(), n));
// }

void Remote::connect(git_direction direction)
{
	Exception::git2_assert(git_remote_connect(data(), direction, nullptr));
}

bool Remote::isConnected()
{
	return git_remote_connected(data()) != 0;
}

void Remote::disconnect()
{
	git_remote_disconnect(data());
}

void Remote::stop()
{
	git_remote_stop(data());
}

// TODO: fix
// bool Remote::list(HeadListCallbackFunction callback)
// {
// 	auto cb = [](git_remote_head *rhead, void *payload)->int
// 	{
// 		HeadListCallbackFunction* callback = (HeadListCallbackFunction*)payload;
// 		if(payload!=nullptr)
// 			return (*callback)(rhead->local!=0, OId(&rhead->oid), OId(&rhead->loid), rhead->name) ? 0 : GIT_EUSER;
// 		else
// 			return 0;
// 	};

// 	int res = git_remote_ls(data(), cb, (void*)&callback);
// 	if(res==GIT_OK)
// 		return true;
// 	else if(res==GIT_EUSER)
// 		return false;
// 	else
// 		Exception::git2_assert(res);
// }


// TODO: fixB
// void Remote::download(TransfertProgressCallbackFunction callback)
// {
// 	auto cb = [](const git_transfer_progress *stats, void *payload)->int
// 	{
// 		TransfertProgressCallbackFunction* callback = (TransfertProgressCallbackFunction*)payload;
// 		if(payload!=nullptr)
// 			(*callback)(stats->total_objects, stats->indexed_objects, stats->received_objects, stats->received_bytes);
// 		return 0;
// 	};

// 	int res = git_remote_download(data(), cb, (void*)&callback);
// 	Exception::git2_assert(res);
// }

// void Remote::UpdateTips()
// {
// 	Exception::git2_assert(git_remote_update_tips(data()));
// }

// bool Remote::isValidUrl(const std::string& url)
// {
// 	return git_remote_valid_url(url.c_str()) != 0;
// }

// bool Remote::isSupportedUrl(const std::string& url)
// {
// 	return git_remote_supported_url(url.c_str()) != 0;
// }

// void Remote::checkCert(bool check)
// {
// 	git_remote_check_cert(data(), check?1:0);
// }

const git_transfer_progress * Remote::stats()
{
	return git_remote_stats(data());
}

git_remote_autotag_option_t Remote::autotag()const
{
	return git_remote_autotag(data());
}

// int Remote::updateFetchhead()const
// {
// 	return git_remote_update_fetchhead(data());
// }

bool Remote::isValidName(const std::string& name)
{
	return git_remote_is_valid_name(name.c_str())?1:0;
}

// void Remote::setUpdateFetchhead(int value)
// {
// 	git_remote_set_update_fetchhead(data(), value);
// }

git_remote* Remote::data() const
{
	return _remote;
}

const git_remote* Remote::constData() const
{
	return _remote;
}


} // namespace git2
