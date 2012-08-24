/* Copyright (c) 2010-2012 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "TestUtil.h"

#include "LogCabinHelper.h"
#include "MockCluster.h"

namespace RAMCloud {

class LogCabinHelperTest : public ::testing::Test {
  public:
    Context context;
    MockCluster cluster;
    LogCabinHelper* logCabinHelper;
    LogCabin::Client::Log* logCabinLog;

    LogCabinHelperTest()
        : context()
        , cluster(context)
        , logCabinHelper()
        , logCabinLog()
    {
        Logger::get().setLogLevels(RAMCloud::SILENT_LOG_LEVEL);

        logCabinHelper = cluster.coordinator.get()->logCabinHelper.get();
        logCabinLog = cluster.coordinator.get()->logCabinLog.get();
    }

    ~LogCabinHelperTest() {
    }

    DISALLOW_COPY_AND_ASSIGN(LogCabinHelperTest);
};

TEST_F(LogCabinHelperTest, appendProtoBuf_and_parseProtoBufFromEntry) {
    ProtoBuf::EntryType entry0;
    entry0.set_entry_type("DummyEntry0");
    EntryId entryId0 = logCabinHelper->appendProtoBuf(entry0);
    EXPECT_EQ(0U, entryId0);

    ProtoBuf::EntryType entry1;
    entry1.set_entry_type("DummyEntry1");
    EntryId entryId1 =
        logCabinHelper->appendProtoBuf(entry1, vector<EntryId>({entryId0}));
    EXPECT_EQ(1U, entryId1);

    vector<Entry> allEntries = logCabinLog->read(0);

    ProtoBuf::EntryType entry0ProtoBuf;
    logCabinHelper->parseProtoBufFromEntry(allEntries[0], entry0ProtoBuf);
    EXPECT_EQ("entry_type: \"DummyEntry0\"\n", entry0ProtoBuf.DebugString());

    ProtoBuf::EntryType entry1ProtoBuf;
    logCabinHelper->parseProtoBufFromEntry(allEntries[1], entry1ProtoBuf);
    EXPECT_EQ("entry_type: \"DummyEntry1\"\n", entry1ProtoBuf.DebugString());

    vector<EntryId> invalidates1 = allEntries[1].getInvalidates();
    EXPECT_FALSE(invalidates1.empty());
}

TEST_F(LogCabinHelperTest, getEntryType) {
    ProtoBuf::EntryType entry0;
    entry0.set_entry_type("DummyEntry0");
    logCabinHelper->appendProtoBuf(entry0);

    vector<Entry> allEntries = logCabinLog->read(0);

    string entryType = logCabinHelper->getEntryType(allEntries[0]);
    EXPECT_EQ("DummyEntry0", entryType);
}

TEST_F(LogCabinHelperTest, readValidEntries) {
    ProtoBuf::EntryType entry0;
    entry0.set_entry_type("DummyEntry0");
    EntryId entryId0 = logCabinHelper->appendProtoBuf(entry0);

    ProtoBuf::EntryType entry1;
    entry1.set_entry_type("DummyEntry1");
    logCabinHelper->appendProtoBuf(entry1);

    ProtoBuf::EntryType entry2;
    entry2.set_entry_type("DummyEntry2");
    logCabinHelper->appendProtoBuf(entry2, vector<EntryId>({entryId0}));

    vector<Entry> validEntries = logCabinHelper->readValidEntries();

    string check = "";
    for (vector<Entry>::iterator it = validEntries.begin();
            it < validEntries.end(); it++) {
        string entryType = logCabinHelper->getEntryType(*it);
        check = format("%sEntryType: %s | ", check.c_str(), entryType.c_str());
    }

    EXPECT_EQ("EntryType: DummyEntry1 | EntryType: DummyEntry2 | ", check);
}

}  // namespace RAMCloud
