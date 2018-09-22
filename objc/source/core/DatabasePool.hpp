/*
 * Tencent is pleased to support the open source community by making
 * WCDB available.
 *
 * Copyright (C) 2017 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use
 * this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *       https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DatabasePool_hpp
#define DatabasePool_hpp

#include <WCDB/Lock.hpp>
#include <WCDB/Path.hpp>
#include <WCDB/RecyclableDatabase.hpp>

namespace WCDB {

class DatabasePoolEvent {
protected:
    virtual void onDatabaseCreated(Database* database) = 0;
    friend class DatabasePool;
};

class DatabasePool {
#pragma mark - DatabasePool
public:
    DatabasePool();

    template<typename T>
    RecyclableDatabase getOrCreate(const std::string& path)
    {
        std::string normalized = Path::normalize(path);
        {
            SharedLockGuard lockGuard(m_lock);
            RecyclableDatabase database = get(normalized);
            if (database != nullptr) {
                return database;
            }
        }
        LockGuard lockGuard(m_lock);
        RecyclableDatabase database = get(normalized);
        if (database != nullptr) {
            return database;
        }
        return add(normalized, std::shared_ptr<Database>(new T(normalized)));
    }

    RecyclableDatabase get(const std::string& path);
    RecyclableDatabase get(const Tag& tag);

    void purge();

protected:
    struct ReferencedDatabase {
        ReferencedDatabase(std::shared_ptr<Database>&& database);
        std::shared_ptr<Database> database;
        int reference;
    };
    typedef struct ReferencedDatabase ReferencedDatabase;

    RecyclableDatabase add(const std::string& path, std::shared_ptr<Database>&& database);
    RecyclableDatabase
    get(const std::map<std::string, ReferencedDatabase>::iterator& iter);
    void flowBack(Database* database);

    std::map<std::string, ReferencedDatabase> m_databases; //path->{database, reference}
    SharedLock m_lock;

#pragma mark - Event
public:
    void setEvent(DatabasePoolEvent* event);

protected:
    DatabasePoolEvent* m_event;
    void onDatabaseCreated(Database* database);
};

} //namespace WCDB

#endif /* DatabasePool_hpp */