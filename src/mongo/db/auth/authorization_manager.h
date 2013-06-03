/**
*    Copyright (C) 2013 10gen Inc.
*
*    This program is free software: you can redistribute it and/or  modify
*    it under the terms of the GNU Affero General Public License, version 3,
*    as published by the Free Software Foundation.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <boost/scoped_ptr.hpp>
#include <string>

#include "mongo/base/disallow_copying.h"
#include "mongo/base/status.h"
#include "mongo/db/auth/authz_manager_external_state.h"
#include "mongo/db/auth/user_name.h"
#include "mongo/db/jsobj.h"

namespace mongo {

    /**
     * Internal secret key info.
     */
    struct AuthInfo {
        AuthInfo();
        std::string user;
        std::string pwd;
    };
    extern AuthInfo internalSecurity; // set at startup and not changed after initialization.

    /**
     * Contains server/cluster-wide information about Authorization.
     */
    class AuthorizationManager {
        MONGO_DISALLOW_COPYING(AuthorizationManager);
    public:

        // The newly constructed AuthorizationManager takes ownership of "externalState"
        explicit AuthorizationManager(AuthzManagerExternalState* externalState);

        static const std::string SERVER_RESOURCE_NAME;
        static const std::string CLUSTER_RESOURCE_NAME;

        static const std::string USER_NAME_FIELD_NAME;
        static const std::string USER_SOURCE_FIELD_NAME;
        static const std::string PASSWORD_FIELD_NAME;

        // TODO: Make the following functions no longer static.

        /**
         * Sets whether or not we allow old style (pre v2.4) privilege documents for this whole
         * server.
         */
        static void setSupportOldStylePrivilegeDocuments(bool enabled);

        /**
         * Returns true if we allow old style privilege privilege documents for this whole server.
         */
        static bool getSupportOldStylePrivilegeDocuments();

        /**
         * Sets whether or not access control enforcement is enabled for this whole server.
         */
        static void setAuthEnabled(bool enabled);

        /**
         * Returns true if access control is enabled on this server.
         */
        static bool isAuthEnabled();

        AuthzManagerExternalState* getExternalState() const;

        // Gets the privilege information document for "userName" on "dbname".
        //
        // On success, returns Status::OK() and stores a shared-ownership copy of the document into
        // "result".
        Status getPrivilegeDocument(const std::string& dbname,
                                    const UserName& userName,
                                    BSONObj* result) const;

        // Returns true if there exists at least one privilege document in the given database.
        bool hasPrivilegeDocument(const std::string& dbname) const;

    private:

        static bool _doesSupportOldStylePrivileges;

        // True if access control enforcement is enabled on this node (ie it was started with
        // --auth or --keyFile).
        // This is a config setting, set at startup and not changing after initialization.
        static bool _authEnabled;

        scoped_ptr<AuthzManagerExternalState> _externalState;
    };

} // namespace mongo
